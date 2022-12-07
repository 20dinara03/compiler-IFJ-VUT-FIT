#include "parser.h"
#include <string.h>
#include <stdio.h>
#include "expr_parser.h"

#define token self->scanner->current_token
#define next_token safeNextToken(self)
#define token_not_null \
    if (token == NULL) \
    {                  \
        return false;  \
    }
#define token_is(b) _equal_token(self, b)
#define token_is_not_jump(b) _equal_not_jump(token->text, b)
#define token_type_is(b) _equal_token_type(self, b)
#define equal(a, b) strcmp(a, b) == 0

#define logging(a) self->logger->debug(self->logger, a)
#define extra_logging                        \
    if (token != NULL)                       \
    {                                        \
        logging(token->text);                \
        logging("\t(");                      \
        logging(token->decode(token->type)); \
        logging(")");                        \
        logging("\n\t | \t");                \
    }
#define log(a)         \
    char *rule = a;    \
    if (token != NULL) \
    {                  \
        extra_logging; \
        logging(rule); \
        logging("\n"); \
    }                  \
    else               \
    {                  \
        return false;  \
    }
#define indirect_log(a) \
    extra_logging;      \
    logging(a);         \
    logging("\n");

#define None {}
#define AND &&_safe_and(self, rule, NULLPTR, NULLPTR) &&
#define CAND(_t) &&_safe_and(self, rule, _t, NULLPTR) &&   // code &&
#define JAND(_t) &&_safe_and(self, rule, JUMPIFNEQ, _t) && // jump &&
#define OR || _safe_or(self, rule) ||

#define sym_table self->symbol_table
#define scope_var(name) self->symbol_table->insert(self->symbol_table, name, "0", NIL, false)
#define scope_func(name, type) self->symbol_table->insert(self->symbol_table, name, "0", type, true)
#define scope_find(name) self->symbol_table->find_g(self->symbol_table, name)
#define scope_get(name) self->symbol_table->find(self->symbol_table, name)
#define new_scope(name) self->symbol_table->push_frame(&self->symbol_table, name);
#define end_scope self->symbol_table->pop_frame(&self->symbol_table);
#define register_not_found(name) if (scope_find(name) == BLACKHOLE) {frame_add_line(as DEFVAR(new_arg(TF, name))); \
                                                                scope_var(name); }
#define arg(type, name) new_arg(type, name)
#define simple_arg(name) new_simple_arg(name)
#define label(type) new_label(self->code_stack, type, -1)
#define simple_label(name) new_simple_label(name)

#define frame_add_line self->get_current_block(self)->add_line
#define as self->get_current_block(self), self->get_current_stack(self)->templater->
#define new_code_frame                                              \
    self->get_current_stack(self)->push(self->get_current_stack(self), self->definition_stage);          \
    bool pass;

#define end_code_frame \
    self->get_current_stack(self)->pop(self->get_current_stack(self), self->definition_stage);

define_logging(parser)

arg_type retype(types_t token_type){
    switch(token_type){
        case INT_LITERAL:
            return INT;
        case DOUBLE_LITERAL:
            return FLOAT;
        case STRING_LITERAL:
            return STRING;
        case KEYWORD_NULL:
            return NIL;
        default:
            return NIL;
    }
}

code_block_t* get_active_code_block(parser_t* self) {
    return self->get_current_stack(self)->blocks[self->get_current_stack(self)->size - 1];
}

code_stack_t *get_active_code_stack(parser_t *self) {
    return self->definition_stage ? self->function_code_stack : self->code_stack;
}

parser_t* init_parser(scanner_t* scanner) {
    parser_t *self = malloc(sizeof(parser_t));
    self->logger = init_parser_logging();
    self->scanner = scanner;
    self->code_stack = new_code_stack();
    self->function_code_stack = new_code_stack(); // GIGA OOP
    self->definition_stage = false;
    self->get_current_block = get_active_code_block;
    self->get_current_stack = get_active_code_stack;
    self->counter = 0;

    if (!self->scanner->logger->active && !self->logger->active)
        self->code_stack->active = true;

    self->symbol_table = init_symbol_table(GF);

    self->parse = parse;
    self->free = destruct_parser;
    return self;
}

arg_type token_to_type(types_t type)
{
    switch (type)
    {
    case KEYWORD_INT: case INT_LITERAL:
        return INT;
    case KEYWORD_FLOAT: case DOUBLE_LITERAL:
        return FLOAT;
    case KEYWORD_STRING: case STRING_LITERAL:
        return STRING;
    default:
        return NIL;
    }
}

bool _equal(parser_t *self, const char *a, const char *b)
{
    if (strcmp(a, b) == 0)
    {
        token->free(&token);
        next_token;
        return true;
    }
    return false;
}

bool _equal_token(parser_t *self, const char *b)
{
    if (token != NULL)
        return _equal(self, token->text, b);
    return false;
}

bool _equal_token_type(parser_t *self, const char *t)
{
    if (token != NULL)
        return _equal(self, token->decode(token->type), t);
    return false;
}

bool _equal_not_jump(const char *a, const char *b)
{
    return strcmp(a, b) == 0;
}

void tryTrashToken(parser_t *self)
{
    if (token == NULL)
        return; // last token check // error check
    if (token_type_is("COMMENT") || token_type_is("COMMENT_LINE"))
        tryTrashToken(self); // trash comments
}

token_t *safeNextToken(parser_t *self)
{
    token_t *next = self->scanner->get_next_token(self->scanner);
    tryTrashToken(self);
    return next;
}

void destruct_parser(parser_t *self)
{
    if (self->scanner != NULL)
        self->scanner->free(self->scanner);
    if (self->logger != NULL)
        free_parser_logging(self->logger);
    if (self->code_stack != NULL)
        self->code_stack->free(self->code_stack);
    if (self->function_code_stack != NULL)
        self->function_code_stack->free(self->function_code_stack);
    if (self->symbol_table != NULL)
        self->symbol_table->free(&self->symbol_table);
    if (self != NULL)
        free(self);
}

bool parse(parser_t *self)
{
    next_token;
    tryTrashToken(self);
    token_not_null if (parseProg(self) && token == NULL)
    { // check all tokens are gone
        logging("\33[1;32mPARSING SUCCESS\33[0m\n");
        return true;
    }
    logging("\33[1;31mPARSING FAILED\33[0m\n");
    exit_failure(SYNTAXIS_ANALYSIS_ERR);
    return false;
}

bool _safe_and(parser_t *self, char *rule, code_type type, code_type type2)
{
    if (type == JUMPIFNEQ)
        frame_add_line(as JUMPIFNEQ(label(type2), arg(TF, RESULT), arg(BOOL, "true")));
    else if (type == IF || type == WHILE)
        frame_add_line(as LABEL(label(type)));
    else if (type == ELSE) {
        frame_add_line(as JUMP(label(IF_END)));
        frame_add_line(as LABEL(label(type)));
    }


    if (token != NULL)
    {
        indirect_log(rule)
    }
    else
    {
        return false;
    }
    return true;
}

bool _safe_or(parser_t *self, char *rule)
{
    indirect_log(rule) return false;
}

bool parseIdentifier(parser_t *self)
{
    token_not_null
    log("parseIdentifier")

    return token_type_is("IDENTIFIER");
}

bool parseVariableIdentifier(parser_t *self, string function, arg_type type)
{
    token_not_null
    log("parseVariableIdentifier");

    char param[strlen(token->text)];
    strcpy(param, token->text);

    bool pass = token_type_is("VARIABLE_IDENTIFIER");

    if (function != NULL)
    {
        symbol_variable_t *func = scope_get(function);
        if (func != NULL)
            func->push_arg(func, param, NULL, type);
        else
            exit_failure(SEMANTIC_UNDEFINED_FUNC_ERR); // undefined function is ST
    }
    else {
        if (pass) {
            frame_add_line(as DEFVAR(new_arg(TF, param)));
            scope_var(param);
        }
    }

    return pass;
}

bool parseProg(parser_t *self)
{
    log("prog ::= '?php' 'declare' '(' 'strict_types' '=' '1' ')' ';' code_lines optional_ending");

    if (token_is("<?php"))
    {
        scope_func("reads", STRING);
        scope_func("readi", INT);
        scope_func("readf", FLOAT);
        scope_func("write", NIL);
        scope_func("floatval", FLOAT);
        scope_func("intval", INT);
        scope_func("strval", STRING);
        scope_func("strlen", INT);
        scope_func("substring", STRING);
        scope_func("ord", INT);
        scope_func("chr", STRING);
        new_code_frame
        printf("%s\n", ".IFJcode22");
        printf("%s\n", "JUMP %main%"); // TODO: as const
        frame_add_line(as LABEL(simple_label("%main%")));
        frame_add_line(as CREATEFRAME());
        frame_add_line(as DEFVAR(new_arg(TF, RESULT)));
        scope_var(RESULT);
        pass = token_is("declare") AND token_is("(") AND token_is("strict_types")
            AND token_is("=") AND token_is("1") AND token_is(")") AND token_is(";")
                AND parseCodeLines(self);
        if (pass)
            pass = parseOptionalEnding(self);
        return pass;
        end_code_frame
    }
    return false;
}

bool parseOptionalEnding(parser_t *self)
{
    if (token != NULL)
        return token_is("?>");
    return true;
}

bool parseCodeLines(parser_t *self)
{
    bool pass = parseCodeLine(self);
    if (pass)
    {
        bool pass2 = parseCodeLines(self);
        return pass2;
    }
    return true;
}

bool parseCodeLine(parser_t *self)
{
    log("code_line ::= statement | function_definition");
    return parseStatement(self) OR parseFunctionDefinition(self);
}

bool parseFunctionDefinition(parser_t *self)
{
    log("function_definition ::= function_header '{' statements '}'");
    self->definition_stage = true;
    new_code_frame;

    arg_type function_type = NIL;

    if (token_is("function"))
    {
        string function_name = malloc(sizeof(char) * (strlen(token->text) + 1));
        strcpy(function_name, token->text);

        if (parseFunctionHeader(self, function_name, &function_type)) {
            new_scope(function_name);

            pass = token_is("{") && parseStatements(self) && token_is("}");

            if (!pass)
                exit_failure(SYNTAXIS_ANALYSIS_ERR);

            frame_add_line(as RETURN());
            end_code_frame;
            end_scope;
            self->definition_stage = false;

            return true;
        }
    }
    end_code_frame;
    self->definition_stage = false;
    return false;
}

bool parseFunctionHeader(parser_t *self, string function_name, arg_type*function_type)
{
    log("function_header ::= 'function' function_identifier '(' function_params ')' optional_result_type");

    code_block_t* cd = self->get_current_block(self);
    code_stack_t *cs = self->get_current_stack(self);

    cd->add_line(cd,cs->templater->LABEL(simple_label(function_name)));

    scope_func(function_name, NIL);
    if (parseIdentifier(self) AND token_is("(") AND parseFunctionParams(self, function_name) AND token_is(")")) {
        parseOptionalResultType(self, function_name, function_type);
        free(function_name);
    } else {
        free(function_name);
        exit_failure(SYNTAXIS_ANALYSIS_ERR);
    }

    return true;
}

bool parseOptionalResultType(parser_t *self, string function, arg_type *function_type)
{
    log("optional_result_type ::= ':' type | ''");

    if (token_is(":"))
    {
        bool pass = parseType(self);

        if (pass)
        {
            symbol_variable_t *func = scope_get(function);
            if (func != NULL)
            {
                func->type = token_to_type(token->type);
                *function_type = func->type;
            }
            else
                exit_failure(SEMANTIC_UNDEFINED_FUNC_ERR); // undefined function is ST
        }
        return pass;
    }
    return false;
}

bool parseFunctionParams(parser_t *self, string function)
{
    log("function_params ::= function_param function_n_param | ''");
    return parseFunctionParam(self, function) ? parseFunctionNParam(self, function) : true;
}

bool parseFunctionNParam(parser_t *self, string function)
{
    log("function_n_param ::= ',' function_param function_n_param | ''");
    return token_is(",") ? (parseFunctionParam(self, function) && parseFunctionNParam(self, function)) : true;
}

bool parseFunctionParam(parser_t *self, string function)
{
    log("function_param ::= type variable_identifier");
    types_t token_type = token->type;

    if (parseType(self))
    {
        char identifier[strlen(token->text)];
        strcpy(identifier, token->text);

        arg_type type = token_to_type(token_type);

        bool pass = parseVariableIdentifier(self, function, type);

        return pass;
    }
    return false;
}

int cmp_numstr(char *p1, char *p2)
{
    while (*p1 && (*p1 == '0'))
        p1++;
   while (*p2 && (*p2 == '0'))
        p2++;
    if (strcmp (p1, p2) == 0)
        return 0;
    else if (strlen(p1) > strlen(p2))
        return 1;
    else if (strlen(p1) < strlen(p2))
        return -1;
    return strcmp(p1, p2);
}

bool parseFunctionCall(parser_t *self)
{
    log("function_call ::= function_identifier '(' variable_func_identifiers ')'");
    char function_name[strlen(token->text) + 1];
    strcpy(function_name, token->text);
    if (parseIdentifier(self))
    {
        bool built_in = (equal(function_name, "write") || equal(function_name, "read") || equal(function_name, "exit")
                || equal(function_name, "reads") || equal(function_name, "readi") || equal(function_name, "readf")
                || equal(function_name, "floatval") || equal(function_name, "intval") || equal(function_name, "strval")
                || equal(function_name, "strlen") || equal(function_name, "substring") || equal(function_name, "ord")
                || equal(function_name, "chr"));
        char param[MAX_CODE_LINE_LENGTH];
        symbol_variable_t *func = scope_get(function_name);
        arg_type type = -1;

        if (token_is("(")) {              // skip "(" token and
            strcpy(param, token->text);
            if (built_in) {
                self->counter++;
                type = token_to_type(token->type);
            }
        } else
            exit_failure(SYNTAXIS_ANALYSIS_ERR);

        bool pass = parseVariableFuncIdentifiers(self, built_in, &func) AND token_is_not_jump(")"); // parse next rules
        if (equal(function_name, "write"))
            None // IS IMPLEMENTED in VariableFuncIdentifier
        else if (equal(function_name, "reads") || equal(function_name, "readi") || equal(function_name, "readf")) {
            register_not_found(RESULT)
            register_not_found(FUN_RETURN)
            frame_add_line(as MOVE(new_arg(TF, FUN_RETURN), new_arg(STRING, "")));
            frame_add_line(as MOVE(new_arg(TF, RESULT), new_arg(STRING, "")));
            frame_add_line(as LABEL(new_label(self->code_stack, BUILT_IN, self->counter)));
            frame_add_line(as READ(new_arg(TF, FUN_RETURN), new_simple_arg("string")));
            frame_add_line(as JUMPIFEQ(new_label(self->code_stack, BUILT_IN_END, self->counter),
                                       new_arg(TF, FUN_RETURN), new_arg(NIL, "nil")));
            frame_add_line(as CONCAT(new_arg(TF, RESULT), new_arg(TF, RESULT),
                                     new_arg(TF, FUN_RETURN)));
            // TODO: check what is comparing with EOF
            frame_add_line(as JUMP(new_label(self->code_stack, BUILT_IN, self->counter)));
            frame_add_line(as LABEL(new_label(self->code_stack, BUILT_IN_END, self->counter)));
            if (equal(function_name, "readi") || equal(function_name, "readf")) {
                frame_add_line(as STRI2INT(new_arg(TF, RESULT), new_arg(TF, RESULT),
                                           new_arg(INT, "0")));
                if (equal(function_name, "readf"))
                    frame_add_line(as INT2FLOAT(new_arg(TF, RESULT), new_arg(TF, RESULT)));
            }
        }
        else if (equal(function_name, "floatval")) {
            register_not_found(RESULT)
            if (type == STRING) {
                frame_add_line(as STRI2INT(new_arg(TF, RESULT), new_arg(TF, param), new_arg(STRING, "0")));
                frame_add_line(as INT2FLOAT(new_arg(TF, RESULT), new_arg(TF, param)));
            } else if (type == INT) {
                frame_add_line(as INT2FLOAT(new_arg(TF, RESULT), new_arg(TF, param)));
            } else if (type == FLOAT) {
                frame_add_line(as MOVE(new_arg(TF, RESULT), new_arg(TF, param)));
            } else
                exit_failure(SEMANIC_OTHER_ERR);
        }
        else if (equal(function_name, "intval")) {
            register_not_found(RESULT)
            if (type == STRING) {
                frame_add_line(as STRI2INT(new_arg(TF, RESULT), new_arg(TF, param), new_arg(STRING, "0")));
            } else if (type == INT) {
                frame_add_line(as MOVE(new_arg(TF, RESULT), new_arg(TF, param)));
            } else if (type == FLOAT) {
                frame_add_line(as FLOAT2INT(new_arg(TF, RESULT), new_arg(TF, param)));
            } else
                exit_failure(SEMANIC_OTHER_ERR);
        }
        else if (equal(function_name, "strval")) {
            register_not_found(RESULT)
            if (type == STRING) {
                frame_add_line(as MOVE(new_arg(TF, RESULT), new_arg(TF, param)));
            } else if (type == INT) {
                frame_add_line(as INT2FLOAT(new_arg(TF, RESULT), new_arg(TF, param)));
            } else if (type == FLOAT) {
                frame_add_line(as FLOAT2INT(new_arg(TF, RESULT), new_arg(TF, param)));
            } else
                exit_failure(SEMANIC_OTHER_ERR);
        }
        else if (equal(function_name, "strlen")) {
            register_not_found(RESULT)
            if (type == STRING) {
                frame_add_line(as STRLEN(new_arg(TF, RESULT), new_arg(TF, param)));
            } else
                exit_failure(SEMANIC_OTHER_ERR);
        }
        else if (equal(function_name, "substring")) {
            register_not_found(RESULT)
            register_not_found(FUN_RETURN)
            if (type == STRING) {
                char* start = func->arg_next->arg_next->name;
                char* end = func->arg_next->arg_next->arg_next->name;
                int _len = (int)strlen(param);
                char* len = malloc(sizeof(char) * (_len + 1));
                sprintf(len, "%d", _len);

                if (cmp_numstr(start, end) <= 0 || cmp_numstr(len, start) <= 0 ||
                    cmp_numstr(len, end) <= 0|| cmp_numstr(start, "0") < 0 || cmp_numstr(end, "0") < 0) {
                    free(len);
                    frame_add_line(as MOVE(new_arg(TF, RESULT), new_arg(STRING, " ")));
                    return pass;
                }
                free(len);

                char* counter = "%BUILT_IN_COUNTER%";
                register_not_found(counter)
                frame_add_line(as MOVE(new_arg(TF, counter), new_arg(INT, start)));
                frame_add_line(as MOVE(new_arg(TF, RESULT), new_arg(STRING, "\\010")));
                frame_add_line(as MOVE(new_arg(TF, FUN_RETURN), new_arg(TF, param)));
                frame_add_line(as LABEL(label(BUILT_IN)));
                frame_add_line(as GETCHAR(new_arg(TF, RESULT), new_arg(TF, FUN_RETURN),
                                          new_arg(TF, start)));
                frame_add_line(as ADD(new_arg(TF, counter), new_arg(TF, counter), new_arg(INT, "1")));
                frame_add_line(as JUMPIFEQ(new_label(self->code_stack, BUILT_IN_END, self->counter),
                                           new_arg(TF, counter), new_arg(INT, end)));
                frame_add_line(as JUMP(new_label(self->code_stack, BUILT_IN, self->counter)));
                frame_add_line(as LABEL(label(BUILT_IN_END)));
            } else
                exit_failure(SEMANIC_OTHER_ERR);
        }
        else if (equal(function_name, "ord")) {
            char* helper = NULL;
            malloc_s(helper, char, 3);
            snprintf(helper, 2, "%c", *param); // read first char as ASCII

            register_not_found(RESULT)
            frame_add_line(as MOVE(new_arg(TF, RESULT), new_arg(INT, helper)));
            free(helper);
        }
        else if (equal(function_name, "chr")) {
            if (type != INT)
                exit_failure(SEMANIC_OTHER_ERR);

            char* helper = NULL;
            int value = (int)strtol(param, NULL, 10);
            malloc_s(helper, char, 3);
            snprintf(helper, 2, "%c", value); // write to helper as ASCII

            register_not_found(RESULT)
            frame_add_line(as MOVE(new_arg(TF, RESULT), new_arg(STRING, helper)));
            free(helper);
        } else if (equal(function_name, "exit")) {
                frame_add_line(as EXIT(simple_arg(param)));
            }
        else {
            if (self->symbol_table->find_g(self->symbol_table, RESULT) == BLACKHOLE) {
                frame_add_line(as DEFVAR(new_arg(TF, RESULT)));
                scope_var(RESULT);
            }
            frame_add_line(as PUSHFRAME());
            frame_add_line(as CREATEFRAME());
            frame_add_line(as CALL(simple_label(function_name)));
            frame_add_line(as POPFRAME());
        }

        return pass;
    }
    return false;
}

bool parseVariableFuncIdentifiers(parser_t *self, bool built_in, symbol_variable_t** func)
{
    log("variable_func_identifiers ::= variable_func_identifier variable_n_identifier | ''");
    return parseVariableFuncIdentifier(self, built_in, func) ? parseVariableNIdentifier(self, built_in, func) : true;
}

bool parseVariableNIdentifier(parser_t *self, bool built_in, symbol_variable_t** func)
{
    log("variable_n_identifier ::= ',' variable_func_identifier variable_n_identifier | ''");
    return token_is(",") ? (parseVariableFuncIdentifier(self, built_in, func) && parseVariableNIdentifier(self, built_in, func)) : true;
}

bool parseVariableFuncIdentifier(parser_t *self, bool built_in, symbol_variable_t** func)
{
    log("variable_func_identifier ::= variable_identifier | expression");

    char identifier[strlen(token->text) + 1];
    strcpy(identifier, token->text);

    types_t token_type = token->type;

    if (parseVariableIdentifier(self, NULL, NIL))
    {
        if (!built_in) {
            if (*func != NULL) {
                frame_add_line(as MOVE(new_arg(TF, (*func)->arg_next->name), new_arg(LF, identifier)));
                *func = (*func)->arg_next;
            } else {
                exit_failure(SEMANTIC_UNDEFINED_FUNC_ERR); // undefined function is ST
            }
        } else if ((*func)->name != NULL || strcmp((*func)->name, "write") == 0) {
            frame_add_line(as WRITE(new_arg(token_to_type(token_type), identifier)));
        }
        return true;
    }

    else if (parseExpression(self, FUNC_VARIABLE, identifier))
    {
        if ((token_type == STRING_LITERAL || token_type == INT_LITERAL || token_type == DOUBLE_LITERAL) && !built_in)
        {
            if (*func != NULL) {
                frame_add_line(as MOVE(new_arg(TF, (*func)->arg_next->name), new_arg(token_to_type(token_type), identifier)));
                *func = (*func)->arg_next;
            } else {
                exit_failure(SEMANTIC_UNDEFINED_FUNC_ERR); // undefined function is ST
            }
        }
        else if (!built_in) {
            if (*func != NULL) {
                frame_add_line(as MOVE(new_arg(TF, (*func)->arg_next->name), new_arg(LF, RESULT)));
                *func = (*func)->arg_next;
            } else {
                exit_failure(SEMANTIC_UNDEFINED_FUNC_ERR); // undefined function is ST
            }
        } else if ( (built_in && (*func)->name != NULL) || strcmp((*func)->name, "write") == 0) {
            frame_add_line(as WRITE(new_arg(token_to_type(token_type), identifier)));
        }
        return true;
    }
    return false;
}

bool parseStatements(parser_t *self)
{
    log("statements ::= statement statements | ''");
    bool pass = parseStatement(self) ? parseStatements(self) : true;
    return pass;
}

bool parseStatement(parser_t *self)
{
    log("statement ::= strict_statement ';' | optional_statement");
    // TODO: check parse strict statement without ';' then we can parse optional statement and get true
    bool pass = (parseStrictStatement(self) AND token_is(";")) OR parseOptionalStatement(self);
    return pass;
}

bool parseStrictStatement(parser_t *self)
{
    log("strict_statement ::= identifier_assignment | expression | return");
    bool pass = parseIdentifierAssignment(self) OR parseExpression(self, STATEMENT, NULL) OR parseReturn(self);
    return pass;
}
bool parseOptionalStatement(parser_t *self)
{
    log("optional_statement ::= condition | while");
    bool pass = parseCondition(self) OR parseWhile(self);
    return pass;
}

bool parseWhile(parser_t *self)
{
    log("while ::= 'while' '(' expression ')' '{' statements '}'");

    if (token_is("while"))
    {
        new_scope("while");
        new_code_frame;
        frame_add_line(as LABEL(label(WHILE)));

        pass = token_is("(") AND parseExpression(self, CONDITION, NULL) AND token_is(")");

        frame_add_line(as JUMPIFNEQ(label(WHILE_END), new_arg(TF, RESULT), new_arg(STRING, "bool@true")));

        pass = pass && token_is("{") AND parseStatements(self) AND token_is("}");

        frame_add_line(as JUMP(label(WHILE)));
        frame_add_line(as LABEL(label(WHILE_END)));
        end_code_frame;
        end_scope;

        if (!pass)
            exit_failure(SYNTAXIS_ANALYSIS_ERR);

        return true;
    }
    return false;
}

bool parseCondition(parser_t *self)
{
    log("condition ::= 'if' '(' expression ')' '{'  statements '}' condition_else");

    if (token_is("if"))
    {
        new_code_frame;
        pass = token_is("(") AND parseExpression(self, CONDITION, NULL) AND token_is(")") JAND(ELSE)
            token_is("{") AND parseStatements(self) AND token_is("}");

        pass = pass CAND(ELSE) parseConditionElse(self);
        frame_add_line(as LABEL(label(IF_END)));

        end_code_frame

        if (!pass)
            exit_failure(SYNTAXIS_ANALYSIS_ERR);

        return true;
    }
    return false;
}

bool parseConditionElse(parser_t *self)
{
    log("condition_else ::= 'else' '{' statements '}' | ''");
    if (token_is("else"))
    {
        if ((token_is("{") && parseStatements(self) && token_is("}")))
            return true;
        else
            exit_failure(SYNTAXIS_ANALYSIS_ERR);
    }
    return true;
}

bool parseIdentifierAssignment(parser_t *self)
{
    log("identifier_assignment ::= variable_identifier assignment");
    char variable_name[strlen(token->text) + 1];
    strcpy(variable_name, token->text);
    if (parseVariableIdentifier(self, NULL, NIL))
    {
        bool pass = parseAssignment(self, variable_name);

        if (!pass)
            exit_failure(SYNTAXIS_ANALYSIS_ERR);

        code_block_t * cd = self->get_current_block(self);

        cd->add_line(as MOVE(new_arg(TF, variable_name), new_arg(TF, RESULT)));
        return pass;
    }
    return false;
}

bool parseAssignment(parser_t *self, string variable_name)
{
    log("assignment ::= '=' expression | '+=' expression | '-=' expression | '*=' expression | '/=' expression");

    bool pass = token_is("=") AND parseExpression(self, ASSINGNMENT, variable_name);
    return pass;
}

bool parseReturn(parser_t *self)
{
    log("return ::= 'return' expression")

    if (token_is("return"))
    {
        string variable_name;
        if (self->symbol_table->frame_type == GF){
            variable_name = NULL;
        }
        else{
            variable_name = self->symbol_table->frame_name;
        }
        bool pass = parseExpression(self, RETURN, variable_name);//function name

        frame_add_line(as MOVE(new_arg(LF, RESULT), new_arg(TF, RESULT)));
        if (!self->definition_stage)
            frame_add_line(as EXIT(new_arg(TF, RESULT)));

        return pass;
    }
    return false;
}

bool parseExpression(parser_t *self, expr_type_t expr_type, string variable_name)
{
    log("expression");
    new_code_frame;
    pass = expression(self, expr_type, variable_name);
    end_code_frame;
    return pass;
}

bool parseType(parser_t *self)
{
    char *rule = "type ::= 'int' | 'float' | 'string' | 'bool' | '?int' | '?float' | '?string' | '?bool'";
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
