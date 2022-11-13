#include "token.h"

/* token types in string representation */
const char *TOKEN_TYPE_TO_STRING[] = {
    "KEYWORD",
    "VARIABLE_IDENTIFIER",
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
    "EXPONENTA",
    "OPERATOR_PLUS",
    "OPERATOR_MINUS",
    "OPERATOR_MULTIPLY",
    "OPERATOR_DIVIDE",
    "OPERATOR_LEFT_BRACKET",
    "OPERATOR_RIGHT_BRACKET",
    "OPERATOR_POINT",
    "OPERATOR_GREATER",
    "OPERATOR_LESS",
    "OPERATOR_EQUAL",
    "OPERATOR_LESS_OR_EQUAL",
    "OPERATOR_GREATER_OR_EQUAL",
    "OPERATOR_NOT",
    "OPERATOR_NOT_EQUAL",
    "OPERATOR_OR",
    "OPERATOR_AND",
    "OPERATOR_SEMICOLON",
    "OPERATOR_COLON",
    "OPERATOR_COMMA",
    "OPERATOR_LEFT_CURLY_BRACKET",
    "OPERATOR_RIGHT_CURLY_BRACKET",
    "EXPONENTAL_OPERATOR",
    "OPERATOR_SLASH",
    "HEAD",
    "END",
    "POSSIBLE_END",
    "TYPE_IDENTIFIER",
    "HEXADECIMAL_NUMBER_X",
    "ESCAPE_SEQUENCE",
    "HEXADECIMAL_NUMBER_1",
    "HEXADECIMAL_NUMBER",
    "OCTAVE_NUMBER",
    "OCTAVE_NUMBER_1",
    "OCTAVE_NUMBER_2",
    "KEYWORD_IF",
    "KEYWORD_ELSE",
    "KEYWORD_FLOAT",
    "KEYWORD_FUNCTION",
    "KEYWORD_INT",
    "KEYWORD_NULL",
    "KEYWORD_RETURN",
    "KEYWORD_VOID",
    "KEYWORD_WHILE",
    "KEYWORD_STRING"
};

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

const char* token_type_to_string(types_t type)
{
    return TOKEN_TYPE_TO_STRING[type];
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
    token->decode = token_type_to_string;
    token->free = free_token;

    return token;
}
