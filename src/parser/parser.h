#ifndef PARSE_H
#define PARSE_H
#include "../scanner/realscanner.h"
#include "../common/logging.h"
#include "../code_generation/code_generation_base.h"
#include "../code_generation/code_templates.h"
#include "../common/symbol-table.h"

declare_logging(parser)

typedef struct parser_t {
    code_stack_t *code_stack;
    code_block_t *current_block;

    scanner_t *scanner;
    symbol_table_t *symbol_table;
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
bool parseFunctionHeader(parser_t *self);
bool parseOptionalResultType(parser_t *self);
bool parseFunctionParams(parser_t *self, char* function);
bool parseFunctionNParam(parser_t *self, char *function);
bool parseFunctionParam(parser_t *self, char *function);
bool parseFunctionCall(parser_t *self);
bool parseVariableFuncIdentifiers(parser_t *self, bool);
bool parseVariableNIdentifier(parser_t *self, bool);
bool parseVariableFuncIdentifier(parser_t *self, bool);
bool parseStatements(parser_t *self);
bool parseStatement(parser_t *self);
bool parseStrictStatement(parser_t *self);
bool parseOptionalStatement(parser_t *self);
bool parseWhile(parser_t *self);
bool parseCondition(parser_t *self);
bool parseConditionElse(parser_t *self);
bool parseIdentifierAssignment(parser_t *self);
bool parseAssignment(parser_t *self);
bool parseReturn(parser_t *self);
bool parseExpression(parser_t *self);
bool parseType(parser_t *self);

parser_t* init_parser(scanner_t* scanner);
void destruct_parser(parser_t *self);

#endif //PARSE_H
