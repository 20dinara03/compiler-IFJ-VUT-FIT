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
    "COMMENT"};

/**
 * @brief Token clean up functio
 */
#define free_token(token)               \
    do                                  \
    {                                   \
        free(((token_t *)token)->text); \
        free((token_t *)token);         \
    } while (0)

/**
 * @brief Constructor for token_t
 *
 * @param token target token
 */
token_t *init_token()
{

    token_t *token = NULL;

    memo_allocate(token, sizeof(token_t));

    token->type = WHITE_SPACE;

    return token;
}

/**
 * @brief Appends symbol at the end of the token text
 *
 * @param token target token
 * @param ch input symbol
 */
void push_char_in_token(token_t *token, char ch)
{
    if (token->text == NULL)
    {
        memo_allocate(token->text, sizeof(char) * 2);
        token->text[0] = ch;
        token->text[1] = '\0';
    }
    else
    {
        const size_t length = strlen(token->text);
        memo_reallocate(token->text, length + 2);
        token->text[length] = ch;
        token->text[length + 1] = '\0';
    }
}

/**
 * @brief Inserts into the target queue and resets given token
 *
 * @param queue target queue
 * @param token current token
 */
void push_token_in_queue(queue_t *queue, token_t **token)
{
    enqueue(queue, *token);
    *token = init_token();
}

/**
 * @brief Prints token in console in (type : value) format
 * 
 * @param token 
 */
void print_token(token_t *token)
{
    printf("( %s : %s )\n", TOKEN_TYPE_TO_STRING[token->type], token->text);
}