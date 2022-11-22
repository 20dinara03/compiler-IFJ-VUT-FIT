#include "expr_parser.h"

ast_node_t* build_tree(ast_node_t *self) {
    self->parser->scanner->get_next_token(self->parser->scanner);
    return Expression(self);
}

ast_node_t* CreateNode(token_t *token, ast_node_t* left, ast_node_t* right)
{
    ast_node_t* node = malloc(sizeof(ast_node_t));
    node->token = token;
    node->left = left;
    node->right = right;

    return node;
}

ast_node_t* CreateUnaryNode(token_t *token, ast_node_t* left)
{
    ast_node_t* node = malloc(sizeof(ast_node_t));
    node->token = token;
    node->left = left;
    node->right = NULL;

    return node;
}


ast_node_t* CreateNodeNumber(token_t *token)
{
    ast_node_t* node = malloc(sizeof(ast_node_t));
    node->token = token;
    node->left = NULL;
    node->right = NULL;

    return node;
}

ast_node_t* Expression(ast_node_t *self)
{
    ast_node_t* tnode = Term(self);
    ast_node_t* e1node = Expression1(self);

    token_t *token = init_token();
    token->fill(token, "+", OPERATOR_PLUS);

    return CreateNode(token, tnode, e1node);
}

ast_node_t* Expression1(ast_node_t *self)
{
    ast_node_t* tnode;
    ast_node_t* e1node;

    switch(self->parser->scanner->current_token->type)
    {
    case OPERATOR_PLUS:
        self->parser->scanner->get_next_token(self->parser->scanner);
        tnode = Term(self);
        e1node = Expression1(self);
        

        return CreateNode(self->parser->scanner->current_token, e1node, tnode);

    case OPERATOR_MINUS:
        self->parser->scanner->get_next_token(self->parser->scanner);
        tnode = Term(self);
        e1node = Expression1(self);

        return CreateNode(self->parser->scanner->current_token, e1node, tnode);
    default:
        break;
    }
    token_t *token = init_token();
    token->fill(token, "0", INT_LITERAL);
    return CreateNodeNumber(token);
}

ast_node_t* Term(ast_node_t *self)
{
    ast_node_t* fnode = Factor(self);
    ast_node_t* t1node = Term1(self);

    token_t *token = init_token();
    token->fill(token, "*", OPERATOR_MULTIPLY);

    return CreateNode(token, fnode, t1node);
}

ast_node_t* Term1(ast_node_t *self)
{
    ast_node_t* fnode;
    ast_node_t* t1node;

    switch(self->parser->scanner->current_token->type)
    {
    case OPERATOR_MULTIPLY: 
        self->parser->scanner->get_next_token(self->parser->scanner);
        fnode = Factor(self);
        t1node = Term1(self);
        return CreateNode(self->parser->scanner->current_token, t1node, fnode);

    case OPERATOR_DIVIDE:
        self->parser->scanner->get_next_token(self->parser->scanner);
        fnode = Factor(self);
        t1node = Term1(self);
        return CreateNode(self->parser->scanner->current_token, t1node, fnode);
    default:
        break;
    }
    token_t *token = init_token();
    token->fill(token, "1", INT_LITERAL);
    return CreateNodeNumber(token);
}

ast_node_t* Factor(ast_node_t *self)
{
    ast_node_t* node;
    token_t *token = init_token();
    switch(self->parser->scanner->current_token->type)
    {
    case OPERATOR_LEFT_BRACKET:
        self->parser->scanner->get_next_token(self->parser->scanner);
        node = Expression(self);
        if(self->parser->scanner->current_token->type == OPERATOR_RIGHT_BRACKET)
            self->parser->scanner->get_next_token(self->parser->scanner);
        else
        {
            self->is_build = false;
        }
        return node;

    case OPERATOR_MINUS:
        self->parser->scanner->get_next_token(self->parser->scanner);
        node = Factor(self);
        return CreateUnaryNode(self->parser->scanner->current_token, node);

    case INT_LITERAL:
    

        token->fill(token, token->text, INT_LITERAL);
        self->parser->scanner->get_next_token(self->parser->scanner);
        return CreateNodeNumber(token);
    
    case VARIABLE_IDENTIFIER:

        token->fill(token, token->text, VARIABLE_IDENTIFIER);
        self->parser->scanner->get_next_token(self->parser->scanner);
        return CreateNodeNumber(token);

    case IDENTIFIER:
        token->fill(token, token->text, IDENTIFIER);
        self->parser->scanner->get_next_token(self->parser->scanner);
        return CreateNodeNumber(token);
    
    default:
        
        self->is_build = false;
        return NULL;
    
    }
}

ast_node_t* init_tree(parser_t *parser) {
    ast_node_t* self= NULL;
    memo_allocate(self,ast_node_t,1);
    self->parser = parser;
    self->is_build = true;
    self->build_tree = build_tree;
    return self;
}
