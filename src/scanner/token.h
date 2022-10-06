#include "../common/queue.h"


#ifndef TOKEN
#define TOKEN

/* token types constants */
typedef enum
{
    KEYWORD,
    OPERATOR,
    IDENTIFIER,
    STRING_LITERAL,
    INT_LITERAL,
    DOUBLE_LITERAL,
    WHITE_SPACE,
    COMMENT,
    POSSIBLE_KEYWORD,
    ID_NAME
} types_t;

/* token struct */
typedef struct
{
    string text;
    types_t type;
    size_t offsetX;
    size_t offsetY;
} token_t;

// /* prototypes */
token_t *init_token();
void print_token(token_t *);
void push_char_in_token(token_t *, char );
void push_token_in_queue(queue_t *, token_t **);

#endif
