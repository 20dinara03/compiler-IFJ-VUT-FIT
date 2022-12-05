/*
 * Implementace překladače imperativního jazyka IFJ22
 *
 *      Oleksandr Turytsia  | xturyt00 | team-leader
 *      Mark Barzali        | xbarza00
 *      Dinara Garipova     | xgarip00
 *      Kateryna Zdebska    | xzdebs00
 */
#ifndef PARSE_H
#define PARSE_H
#include "../scanner/realscanner.h"
#include "../common/logging.h"
#include "../code_generation/code_generation_base.h"
#include "../code_generation/code_templates.h"
#include "../common/symbol-table.h"

declare_logging(parser)

    typedef enum {
        FUNC_VARIABLE,
        RETURN,
        ASSINGNMENT,
        CONDITION,
        STATEMENT
    } expr_type_t;

typedef struct parser_t {
    code_stack_t *code_stack;
    code_stack_t *function_code_stack;
    code_block_t *current_block;
    symbol_table_t *symbol_table;
    bool definition_stage;
    code_block_t* (*get_current_block)(struct parser_t *self);
    code_stack_t* (*get_current_stack)(struct parser_t *self);
    int counter;

    scanner_t *scanner;
    parser_logging_t *logger;
    void (*free)(struct parser_t *self);
    bool (*parse)(struct parser_t *self);
} parser_t;

bool equal(parser_t* self, const char* a, const char *b);
void tryTrashToken(parser_t *self);
token_t* safeNextToken(parser_t *self);

bool parse(parser_t *self);
bool parseProg(parser_t *self);
bool parseOptionalEnding(parser_t *self);
bool parseCodeLines(parser_t *self);
bool parseCodeLine(parser_t *self);
bool parseFunctionDefinition(parser_t *self);
bool parseFunctionHeader(parser_t *self, char *function_name, arg_type *function_type);
bool parseOptionalResultType(parser_t *self, char* function_name, arg_type* function_type);
bool parseFunctionParams(parser_t *self, char* function_name);
bool parseFunctionNParam(parser_t *self, char *function_name);
bool parseFunctionParam(parser_t *self, char *function_name);
bool parseFunctionCall(parser_t *self);
bool parseVariableFuncIdentifiers(parser_t *self, bool, symbol_variable_t** func);
bool parseVariableNIdentifier(parser_t *self, bool, symbol_variable_t** func);
bool parseVariableFuncIdentifier(parser_t *self, bool, symbol_variable_t** func);
bool parseStatements(parser_t *self);
bool parseStatement(parser_t *self);
bool parseStrictStatement(parser_t *self);
bool parseOptionalStatement(parser_t *self);
bool parseWhile(parser_t *self);
bool parseCondition(parser_t *self);
bool parseConditionElse(parser_t *self);
bool parseIdentifierAssignment(parser_t *self);
bool parseAssignment(parser_t *self, string variable_name);
bool parseReturn(parser_t *self);
bool parseExpression(parser_t *self, expr_type_t expr_type, string variable_name);
bool parseType(parser_t *self);

parser_t* init_parser(scanner_t* scanner);
void destruct_parser(parser_t *self);

#endif //PARSE_H
