#ifndef EXRP_PARSER
#define EXPR_PARSER

#include "parser.h"

typedef struct ast_node_t ast_node_t;

struct ast_node_t
{
    parser_t *parser;
    token_t *token;
    ast_node_t *left;
    ast_node_t *right;
    bool is_build;
    ast_node_t *(*build_tree)(ast_node_t *self);
};

ast_node_t *init_tree(parser_t *parser);
ast_node_t *build_tree(ast_node_t *self);
ast_node_t *CreateNodeNumber(token_t *token);
ast_node_t *CreateNode(token_t *token, ast_node_t *left, ast_node_t *right);
ast_node_t *CreateUnaryNode(token_t *token, ast_node_t *left);
ast_node_t *Expression(ast_node_t *self);
ast_node_t *Expression1(ast_node_t *self);
ast_node_t *Term(ast_node_t *self);
ast_node_t *Term1(ast_node_t *self);
ast_node_t *Factor(ast_node_t *self);

#endif
