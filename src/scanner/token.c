#include "token.h"

/* token types in string representation */
const char *TOKEN_TYPE_TO_STRING[] = {
    "KEYWORD",
    "OPERATOR",
    "IDENTIFIER",
    "STRING_LITERAL",
    "INT_LITERAL",
    "DOUBLE_LITERAL",
    "WHITE_SPACE",
    "COMMENT",
    "POSSIBLE_KEYWORD",
    "POSSIBLE_OR",
    "POSSIBLE_AND",
    "POSSIBLE_COMMENT",
    "POSSIBLE_COMMENT_END",
    "COMMENT_LINE",
    "KEYWORD_h",
    "KEYWORD_p",
    "EXPONENTA"};

input_node_t *init_input_node(token_t *token)
{
    input_node_t *node = NULL;

    memo_allocate(node, input_node_t, 1);

    node->token = token;

    return node;
}

void push_token_in_queue(input_stack_t *self, token_t *token)
{
    if (self == NULL)
        return;

    input_node_t *new_node = init_input_node(token);

    if (self->head == NULL)
    {
        new_node->next = NULL;
        self->head = new_node;
    }
    else
    {
        new_node->next = self->head;
        self->head = new_node;
    }
}

void free_stack(input_stack_t **self)
{
    if (self == NULL || (*self)->head == NULL)
        return;

    input_node_t *current_node = (*self)->head;
    while (current_node != NULL)
    {
        current_node = current_node->next;
        (*self)->head->token->free(&((*self)->head->token));
        free((*self)->head);
        (*self)->head = current_node;
    }
    free(*self);
}

input_stack_t *init_input_stack()
{
    input_stack_t *stack = NULL;

    memo_allocate(stack, input_stack_t, 1);

    stack->push = push_token_in_queue;
    stack->free = free_stack;

    return stack;
}

void push_char_in_token(token_t *self, char ch)
{
    if (self->text == NULL)
    {
        memo_allocate(self->text, char, 2);
        self->text[0] = ch;
        self->text[1] = '\0';
    }
    else
    {
        const size_t length = strlen(self->text);
        memo_reallocate(self->text, char, length + 2);
        self->text[length] = ch;
        self->text[length + 1] = '\0';
    }
}

void reset_token(token_t **self)
{
    *self = init_token();
}

void debug_token(token_t *self)
{
    printf("( %s : %s )\n", TOKEN_TYPE_TO_STRING[self->type], self->text);
}

void free_token(token_t **self)
{
    free((*self)->text);
    free(*self);
    *self = NULL;
}

token_t *init_token()
{

    token_t *token = NULL;

    memo_allocate(token, token_t, 1);

    token->type = WHITE_SPACE;

    token->push_char = push_char_in_token;
    token->reset = reset_token;
    token->debug = debug_token;
    token->free = free_token;

    return token;
}
