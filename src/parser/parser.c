#include "parser.h"
#include <string.h>
#include <stdio.h>

#define token self->scanner->current_token
#define next_token safeNextToken(self)
#define token_not_null if (token == NULL) {return false;}
#define token_is(b) _equal_token(self, b)
#define token_type_is(b) _equal_token_type(self, b)
#define equal(a, b) strcmp(a, b) == 0

#define logging(a) self->logger->debug(self->logger, a)
#define extra_logging if(token != NULL) {logging(token->text);logging("\t(");logging(token->decode(token->type));logging(")");logging("\n\t | \t");}
#define log(a) char* rule = a; if(token != NULL) {extra_logging; logging(rule); logging("\n");} else { return false; }
#define indirect_log(a) extra_logging; logging(a); logging("\n");

#define None _None(self)
#define AND && _safe_and(self, rule, NULLPTR, NULLPTR) &&
#define CAND(_t) && _safe_and(self, rule, _t, NULLPTR) && // code &&
#define JAND(_t) && _safe_and(self, rule, JUMPIFNEQ, _t) && // jump &&
#define OR || _safe_or(self, rule) ||

#define arg(type, name) new_arg(type, name)
#define simple_arg(name) new_simple_arg(name)
#define label(type) new_label(self->code_stack, type)
#define simple_label(name) new_simple_label(name)

#define frame_add_line self->current_block->add_line
#define as self->current_block, self->code_stack->templater->
#define new_code_frame self->current_block = self->code_stack->push(self->code_stack);  \
        bool pass;

#define end_code_frame                                                                  \
        self->code_stack->pop(self->code_stack, pass);                                  \
        self->current_block = self->code_stack->blocks[self->code_stack->size - 1];

define_logging(parser)

parser_t* init_parser(scanner_t* scanner) {
    parser_t *self = malloc(sizeof(parser_t));
    self->logger = init_parser_logging();
    self->scanner = scanner;
    self->code_stack = new_code_stack();

    if (!self->scanner->logger->active && !self->logger->active)
        self->code_stack->active = true;

    self->parse = parse;
    self->free = destruct_parser;
    return self;
}

bool _equal(parser_t* self, const char* a, const char *b) {
    if (strcmp(a, b) == 0) {
        token->free(&token);
        next_token;
        return true;
    }
    return false;
}

bool _equal_token(parser_t* self, const char *b) {
    if (token != NULL)
        return _equal(self, token->text, b);
    return false;
}

bool _equal_token_type(parser_t* self, const char* t) {
    if (token != NULL)
        return _equal(self, token->decode(token->type), t);
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
    if (self->code_stack != NULL)
        self->code_stack->free(self->code_stack);
    if (self != NULL)
        free(self);
}

bool parse(parser_t *self) {
    next_token; tryTrashToken(self);
    token_not_null
    if (parseProg(self) && token == NULL) { // check all tokens are gone
        logging("\33[1;32mPARSING SUCCESS\33[0m\n");
        return true;
    }
    logging("\33[1;31mPARSING FAILED\33[0m\n");
    return false;
}

bool _safe_and(parser_t *self, char *rule, code_type type, code_type type2) {
    if (type == JUMPIFNEQ)
        frame_add_line(as JUMPIFNEQ(label(type2), arg(TF, RESULT), arg(BOOL, "true")));
    else if (type == IF || type == WHILE)
        frame_add_line(as LABEL(label(type)));

    if (token != NULL) {
        indirect_log(rule)
    } else {
        return false;
    }
    return true;
}

bool _safe_or(parser_t *self, char *rule) {
    indirect_log(rule)
    return false;
}

bool _None(parser_t *self) {
    if (token != NULL) {
        logging("None | for token: ");
        logging(token->text);
        logging("\n");
    }
    return true;
}

bool parseIdentifier(parser_t *self) {
    token_not_null
    // function
    log("parseIdentifier")
    return token_type_is("IDENTIFIER");
}

bool parseVariableIdentifier(parser_t *self) {
    token_not_null
    log("parseVariableIdentifier");
    return token_type_is("VARIABLE_IDENTIFIER");
}

bool parseStringLiteral(parser_t *self) {
    token_not_null
    log("parseStringLiteral");
    // TODO: const literal
    return token_type_is("STRING_LITERAL");
}

bool parseProg(parser_t *self) {
    log("prog ::= '?php' 'declare' '(' 'strict_types' '=' '1' ')' ';' code_lines optional_ending")

    if (token_is("<?php")) {
        new_code_frame
        frame_add_line(as LABEL(simple_label(".IFJcode22")));
        frame_add_line(as JUMP(simple_label("main")));
        pass = token_is("declare") AND token_is("(") AND token_is("strict_types")
               AND token_is("=") AND token_is("1") AND token_is(")") AND token_is(";")
               AND parseCodeLines(self);
        if (pass)
            pass = parseOptionalEnding(self);
        end_code_frame
        return pass;
    }
    return false;
}

bool parseOptionalEnding(parser_t *self) {
    if (token != NULL)
        return token_is("?>");
    return true;
}

bool parseCodeLines(parser_t *self) {
    log("code_lines ::= (code_line code_lines) | ''")
    return parseCodeLine(self) ? parseCodeLines(self) : true;
}

bool parseCodeLine(parser_t *self) {
    log("code_line ::= statement | function_definition")
    return parseStatement(self) OR parseFunctionDefinition(self);
}

bool parseFunctionDefinition(parser_t *self) {
    log("function_definition ::= function_header '{' statements '}'")

    new_code_frame
        pass = parseFunctionHeader(self) AND token_is("{") AND parseStatements(self) AND token_is("}");
    frame_add_line(as RETURN());

    end_code_frame

    return pass;
}

bool parseFunctionHeader(parser_t *self) {
    log("function_header ::= 'function' function_identifier '(' function_params ')' optional_result_type")
    if (token_is("function")) {
        char function[strlen(token->text)];
        strcpy(function, token->text);
        frame_add_line(as LABEL(simple_label(function)));

        bool pass = parseIdentifier(self) AND token_is("(")
                AND parseFunctionParams(self) AND token_is(")") AND parseOptionalResultType(self);
        return pass;
    }
    return false;
}

bool parseOptionalResultType(parser_t *self) {
    log("optional_result_type ::= ':' type | ''")
    return token_is(":") ? parseType(self) : true;
}

bool parseFunctionParams(parser_t *self) {
    log("function_params ::= function_param function_n_param | ''")
    return parseFunctionParam(self) ? parseFunctionNParam(self) : true;
}

bool parseFunctionNParam(parser_t *self) {
    log("function_n_param ::= ',' function_param function_n_param | ''")
    return token_is(",") ? (parseFunctionParam(self) && parseFunctionNParam(self)) : true;
}

bool parseFunctionParam(parser_t *self) {
    log("function_param ::= type variable_identifier")

    if (parseType(self)) {
        char identifier[strlen(token->text)];
        strcpy(identifier, token->text);
        bool pass = parseVariableIdentifier(self);
        frame_add_line(as DEFVAR(new_arg(TF, identifier)));
        frame_add_line(as POPS(new_arg(TF, identifier)));
        return pass;
    }
    return false;
}

bool parseFunctionCall(parser_t *self) {
    log("function_call ::= function_identifier '(' variable_func_identifiers ')'")
    char function_name[strlen(token->text) + 1];
    strcpy(function_name, token->text);
    if (parseIdentifier(self)) {
        bool built_in = (equal(function_name, "write") || equal(function_name, "read") || equal(function_name, "exit"));
        char param[MAX_CODE_LINE_LENGTH];
        if (token_is("("))                                                  // skip "(" token and
            strcpy(param, token->text);                            // save first param in case of empty params

        frame_add_line(as DEFVAR(simple_arg(FUN_RETURN)));
        frame_add_line(as PUSHFRAME());
        frame_add_line(as CREATEFRAME());

        bool pass = parseVariableFuncIdentifiers(self, built_in) AND token_is(")");   // parse next rules

        if (equal(function_name, "write"))
            frame_add_line(as WRITE(new_arg(TF, RESULT)));
        else if(equal(function_name, "read"))
            // TODO: 2nd arg is table->search(token->text)->type
            frame_add_line(as READ(new_arg(TF, RESULT), new_arg(STRING, "int")));
        else if(equal(function_name, "exit"))
            frame_add_line(as EXIT(simple_arg(param)));
        else {
            frame_add_line(as CALL(simple_label(function_name)));
        }

        frame_add_line(as POPFRAME());
        return pass;
    }
    return false;
}

bool parseVariableFuncIdentifiers(parser_t *self, bool built_in) {
    log("variable_func_identifiers ::= variable_func_identifier variable_n_identifier | ''")
    return parseVariableFuncIdentifier(self, built_in) ? parseVariableNIdentifier(self, built_in) : true;
}

bool parseVariableNIdentifier(parser_t *self, bool built_in) {
    log("variable_n_identifier ::= ',' variable_func_identifier variable_n_identifier | ''")
    return token_is(",") ? (parseVariableFuncIdentifier(self, built_in) && parseVariableNIdentifier(self, built_in)) : true;
}

bool parseVariableFuncIdentifier(parser_t *self, bool built_in) {
    log("variable_func_identifier ::= variable_identifier | expression")

    char identifier[strlen(token->text) + 1];
    strcpy(identifier, token->text);

    types_t token_type = token->type;

    if (parseVariableIdentifier(self)) {
        if (!built_in)
            frame_add_line(as PUSHS(new_arg(LF, identifier)));
        return true;
    }

    else if (parseExpression(self)) {
        if (token_type == STRING_LITERAL || token_type == INT_LITERAL || token_type == DOUBLE_LITERAL) {
            frame_add_line(as PUSHS(new_arg(translate_token_type(token_type), identifier)));
        }
        else if (!built_in)
            frame_add_line(as PUSHS(new_arg(TF, RESULT)));
        return true;
    }
    return false;
}

bool parseStatements(parser_t *self) {
    log("statements ::= statement statements | ''")
    return parseStatement(self) ? parseStatements(self) : true;
}

bool parseStatement(parser_t *self) {
    log("statement ::= strict_statement ';' | optional_statement")
    // TODO: check parse strict statement without ';' then we can parse optional statement and get true
    return (parseStrictStatement(self) AND token_is(";")) OR parseOptionalStatement(self);
}

bool parseStrictStatement(parser_t *self) {
    log("strict_statement ::= identifier_assignment | expression | return")
    return parseIdentifierAssignment(self) OR parseExpression(self) OR parseReturn(self);
}

bool parseOptionalStatement(parser_t *self) {
    log("optional_statement ::= condition | while")
    return parseCondition(self) OR parseWhile(self);
}

bool parseWhile(parser_t *self) {
    log("while ::= 'while' '(' expression ')' '{' statements '}'")

    if (token_is("while")) {
        new_code_frame
            frame_add_line(as PUSHFRAME());
            frame_add_line(as LABEL(label(WHILE)));
            pass = token_is("(") AND parseExpression(self) AND token_is(")")
                AND token_is("{") AND parseStatements(self) AND token_is("}");
            frame_add_line(as POPFRAME());
        end_code_frame
        return pass;
    }
    return false;
}

bool parseCondition(parser_t *self) {
    log("condition ::= 'if' '(' expression ')' '{'  statements '}' condition_else")

    if (token_is("if")) {
        new_code_frame
        frame_add_line(as PUSHFRAME());
        frame_add_line(as LABEL(label(IF)));
        pass = token_is("(") AND
       parseExpression(self) AND
               token_is(")") JAND(ELSE)
               token_is("{") AND
       parseStatements(self) AND
               token_is("}") CAND(ELSE)
       parseConditionElse(self);
        frame_add_line(as POPFRAME());
        end_code_frame
        return pass;
    }
    return false;
}

bool parseConditionElse(parser_t *self) {
    log("condition_else ::= 'else' '{' statements '}' | ''")
    return token_is("else") ? (token_is("{") && parseStatements(self) && token_is("}")) : true;
}

bool parseIdentifierAssignment(parser_t *self) {
    log("identifier_assignment ::= variable_identifier assignment")
    char variable_name[strlen(token->text) + 1];
    strcpy(variable_name, token->text);
    if (parseVariableIdentifier(self)) {
        bool pass = parseAssignment(self);
        frame_add_line(as MOVE(new_arg(TF, variable_name), new_arg(TF, RESULT)));
        return pass;
    }
    return false;
}

bool parseAssignment(parser_t *self) {
    log("assignment ::= '=' expression | '+=' expression | '-=' expression | '*=' expression | '/=' expression")
    return token_is("=") AND parseExpression(self);
}

bool parseReturn(parser_t *self) {
    log("return ::= 'return' expression")
    if (token_is("return")) {
        bool pass = parseExpression(self);
        return pass;
    }
    return false;
}

bool parseParenthesis(parser_t *self) {
    log("parenthesis ::= '(' expression ')'")
    return token_is("(") AND parseExpression(self) AND token_is(")");
}

bool UpperExpression(parser_t *self) {
    token_not_null
    char* rule = "cycleExpression ::= expression cycleExpression | ''";
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
    return UpperExpression(self);
}

bool parseType(parser_t *self) {
    char* rule = "type ::= 'int' | 'float' | 'string' | 'bool' | '?int' | '?float' | '?string' | '?bool'";
    return token_is("int") OR token_is("float") OR token_is("string") OR token_is("bool") OR token_is("void")
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
