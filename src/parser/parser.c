#include "parser.h"
#include <string.h>
#include <stdio.h>

#define token self->scanner->current_token
#define next_token safeNextToken(self)
#define logging(a) self->logger->debug(self->logger, a)
#define logging_rule(r) char* rule = #r; logging(rule); logging("\n")
#define token_is(b) equal(self, token->text, b)
#define None _None(self)
#define token_type_is(b) equal(self, token->decode(token->type), b)
#define AND && _safe_and(self, rule, NULLPTR) &&
#define CAND(_t) && _safe_and(self, rule, _t) &&
#define OR || _safe_or(self, rule) ||
#define extra_logging logging(token->text);logging("\t(");logging(token->decode(token->type));logging(")");logging("\n\t | \t")
#define arg(type, name) new_arg(type, name)
#define label(type) new_label(self->code_stack, type))
#define set self->current_block->add_line(self->current_block, self->code_stack->templater->
#define new_code_frame self->current_block = self->code_stack->push(self->code_stack);
#define end_code_frame self->code_stack->pop(self->code_stack, pass); self->current_block = self->code_stack->blocks[self->code_stack->size - 1];


define_logging(parser)

parser_t* init_parser(scanner_t* scanner) {
    parser_t *self = malloc(sizeof(parser_t));
    self->code_stack = new_code_stack();
    self->scanner = scanner;
    self->parse = parse;
    self->logger = init_parser_logging();
    self->free = destruct_parser;
    return self;
}

bool equal(parser_t* self, const char* a, const char *b) {
    if (strcmp(a, b) == 0) {
        free(token);
        next_token;
        return true;
    }
    return false;
}

void tryTrashToken(parser_t *self) {
    if (token == NULL)
        return;                 // last token check // error check
    if (token_type_is("COMMENT") || token_type_is("COMMENT_LINE"))
        tryTrashToken(self);    // trash comments
}

token_t* safeNextToken(parser_t *self) {
    token_t *next = self->scanner->get_next_token(self->scanner);
    tryTrashToken(self);
    return next;
}

void destruct_parser(parser_t *self) {
    if (self->scanner != NULL)
        self->scanner->free(self->scanner);
    if (self->logger != NULL)
        free_parser_logging(self->logger);
    if (self != NULL)
        free(self);
}

bool parse(parser_t *self) {
    next_token; tryTrashToken(self);
    if (parseProg(self) && token == NULL) { // check all tokens are gone
        logging("\33[1;32mPARSING SUCCESS\33[0m\n");
        return true;
    }
    logging("\33[1;31mPARSING FAILED\33[0m\n");
    return false;
}

bool _safe_and(parser_t *self, char *rule, code_type type) {
    if (type != NULLPTR) {
        self->current_block->add_line(self->current_block, self->code_stack->templater->LABEL(new_label(self->code_stack, type)));
    }
    extra_logging; logging(rule); logging("\n");
    return true;
}

bool _safe_or(parser_t *self, char *rule) {
    extra_logging; logging(rule); logging("\n");
    return false;
}

bool _None(parser_t *self) {
    logging("None | for token: "); logging(token->text); logging("\n");
    return true;
}

bool parseIdentifier(parser_t *self) {
    // function
    extra_logging; logging("parseIdentifier\n");
    return token_type_is("IDENTIFIER");
}

bool parseVariableIdentifier(parser_t *self) {
    extra_logging; logging("parseVariableIdentifier\n");
    return token_type_is("VARIABLE_IDENTIFIER");
}

bool parseStringLiteral(parser_t *self) {
    extra_logging; logging("parseStringLiteral\n");
    return token_type_is("STRING_LITERAL");
}

bool parseProg(parser_t *self) {
    extra_logging;
    logging_rule("prog ::= '?php' 'declare' '(' 'strict_types' '=' '1' ')' ';' code_lines optional_ending");
    return token_is("<?php") AND token_is("declare") AND token_is("(") AND token_is("strict_types")
           AND token_is("=") AND token_is("1") AND token_is(")") AND token_is(";")
           AND parseCodeLines(self) AND parseOptionalEnding(self);
}

bool parseOptionalEnding(parser_t *self) {
    extra_logging;
    logging_rule("optional_ending ::= '' | '?'\n");
    return token_is("?>") OR None;
}

bool parseCodeLines(parser_t *self) {
    extra_logging;
	logging_rule("code_lines ::= (code_line code_lines) | ''");
    return (parseCodeLine(self) AND parseCodeLines(self)) OR None;
}

bool parseCodeLine(parser_t *self) {
    extra_logging; 
	logging_rule("code_line ::= statement | function_definition");
    return parseStatement(self) OR parseFunctionDefinition(self);
}

bool parseFunctionDefinition(parser_t *self) {
    extra_logging;
	logging_rule("function_definition ::= function_header '{' statements '}'");

    bool pass = parseFunctionHeader(self) AND token_is("{") AND parseStatements(self) AND token_is("}");

    return pass;
}

bool parseFunctionHeader(parser_t *self) {
    extra_logging; 
	logging_rule("function_header ::= 'function' function_identifier '(' function_params ')' optional_result_type");
    return token_is("function") AND parseIdentifier(self) AND token_is("(")
           AND parseFunctionParams(self) AND token_is(")") AND parseOptionalResultType(self);
}

bool parseOptionalResultType(parser_t *self) {
    extra_logging; 
	logging_rule("optional_result_type ::= ':' type | ''");
    return (token_is(":") AND parseType(self)) OR None;
}

bool parseFunctionParams(parser_t *self) {
    extra_logging; 
	logging_rule("function_params ::= function_param function_n_param | ''");
    return (parseFunctionParam(self) AND parseFunctionNParam(self)) OR None;
}

bool parseFunctionNParam(parser_t *self) {
    extra_logging; 
	logging_rule("function_n_param ::= ',' function_param function_n_param | ''");
    return (token_is(",") AND parseFunctionParam(self) AND parseFunctionNParam(self)) OR None;
}

bool parseFunctionParam(parser_t *self) {
    extra_logging; 
	logging_rule("function_param ::= type variable_identifier");
    return parseType(self) AND parseIdentifier(self);
}

bool parseFunctionCall(parser_t *self) {
    extra_logging; 
	logging_rule("function_call ::= function_identifier '(' variable_func_identifiers ')'");
    return parseIdentifier(self) AND token_is("(") AND parseVariableFuncIdentifiers(self) AND token_is(")");
}

bool parseVariableFuncIdentifiers(parser_t *self) {
    extra_logging; 
	logging_rule("variable_func_identifiers ::= variable_func_identifier variable_n_identifier | ''");
    return (parseVariableFuncIdentifier(self) AND parseVariableNIdentifier(self)) OR None;
}

bool parseVariableNIdentifier(parser_t *self) {
    extra_logging; 
	logging_rule("variable_n_identifier ::= ',' variable_func_identifier variable_n_identifier | ''");
    return (token_is(",") AND parseVariableFuncIdentifier(self) AND parseVariableNIdentifier(self)) OR None;
}

bool parseVariableFuncIdentifier(parser_t *self) {
    extra_logging; 
	logging_rule("variable_func_identifier ::= variable_identifier | expression");
    return parseVariableIdentifier(self) OR parseExpression(self);
}

bool parseStatements(parser_t *self) {
    extra_logging; 
	logging_rule("statements ::= statement statements | ''");
    return (parseStatement(self) AND parseStatements(self)) OR None;
}

bool parseStatement(parser_t *self) {
    extra_logging; 
	logging_rule("statement ::= strict_statement ';' | optional_statement");
    return (parseStrictStatement(self) AND token_is(";")) OR parseOptionalStatement(self);
}

bool parseStrictStatement(parser_t *self) {
    extra_logging; 
	logging_rule("strict_statement ::= identifier_assignment | expression | return");
    return parseIdentifierAssignment(self) OR parseExpression(self) OR parseReturn(self);
}

bool parseOptionalStatement(parser_t *self) {
    extra_logging; 
	logging_rule("optional_statement ::= condition | while");
    return parseCondition(self) OR parseWhile(self);
}

bool parseWhile(parser_t *self) {
    extra_logging; 
	logging_rule("while ::= 'while' '(' expression ')' '{' statements '}'");


    bool pass = token_is("while") AND token_is("(") AND parseExpression(self) AND token_is(")")
        AND token_is("{") AND parseStatements(self) AND token_is("}");

    return pass;
}

bool parseCondition(parser_t *self) {
    extra_logging; 
	logging_rule("condition ::= 'if' '(' expression ')' '{'  statements '}' condition_else");

    new_code_frame
    self->current_block->add_line(self->current_block, self->code_stack->templater->LABEL(new_label(self->code_stack, IF)));
        bool pass = token_is("if") AND token_is("(") AND parseExpression(self) AND token_is(")")
            AND token_is("{") AND parseStatements(self) AND token_is("}") CAND(ELSE) parseConditionElse(self);
    end_code_frame
    return pass;
}

bool parseConditionElse(parser_t *self) {
    extra_logging; 
	logging_rule("condition_else ::= 'else' '{' statements '}' | ''");
    return (token_is("else") AND token_is("{") AND parseStatements(self) AND token_is("}")) OR None;
}

bool parseIdentifierAssignment(parser_t *self) {
    extra_logging;
	logging_rule("identifier_assignment ::= variable_identifier assignment");
    return parseVariableIdentifier(self) AND parseAssignment(self);
}

bool parseAssignment(parser_t *self) {
    extra_logging;
    
	logging_rule("assignment ::= '=' expression | '+=' expression | '-=' expression | '*=' expression | '/=' expression");
    return token_is("=") AND parseExpression(self);
}

bool parseReturn(parser_t *self) {
    extra_logging; 
	logging_rule("return ::= 'return' expression");
    return token_is("return") AND parseExpression(self);
}

bool parseParenthesis(parser_t *self) {
    extra_logging;
    logging_rule("parenthesis ::= '(' expression ')'");
    return token_is("(") AND parseExpression(self) AND token_is(")");
}

bool UpperExpression(parser_t *self) {
    logging_rule("cycleExpression ::= expression cycleExpression | ''");
    if (parseFunctionCall(self)) return true;
    if (
        parseStringLiteral(self) OR parseVariableIdentifier(self) OR token_is("null") OR token_is("===")
        OR token_is("==") OR token_is("!==") OR token_type_is("INT_LITERAL") OR token_type_is("DOUBLE_LITERAL")
        OR token_is("!=") OR token_is(">=") OR token_is("<=") OR token_is(">")
        OR token_is("<") OR token_is("+") OR token_is("-") OR token_is("*") OR token_is("/")
        OR token_is("&&") OR token_is("||") OR token_is("!") OR parseParenthesis(self)
    ) {
        UpperExpression(self); // cycle all other expression
        return true;
    }
    return false;
}

bool parseExpression(parser_t *self) {
    extra_logging;
	logging_rule("expression ::= function_call | string_literal | variable_identifier | ===");
    return UpperExpression(self);
}

bool parseType(parser_t *self) {
    extra_logging; 
	logging_rule("type ::= 'int' | 'float' | 'string' | 'bool' | '?int' | '?float' | '?string' | '?bool'");
    return token_is("int") OR token_is("float") OR token_is("string") OR token_is("bool")
        OR token_is("?int") OR token_is("?float") OR token_is("?string") OR token_is("?bool");
}

#undef token
#undef next_token
#undef logging
#undef logging_rule
#undef token_is
#undef None
#undef token_type_is
#undef AND
#undef OR
#undef extra_logging
