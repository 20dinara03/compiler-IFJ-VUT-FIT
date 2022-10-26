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
