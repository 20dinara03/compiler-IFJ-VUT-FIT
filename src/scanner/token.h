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
    POSSIBLE_OR,
    POSSIBLE_AND,
    POSSIBLE_COMMENT,
    POSSIBLE_COMMENT_END,
    COMMENT_LINE
} types_t;

/* token struct */
typedef struct
{
    string text;
    types_t type;
    size_t offsetX;
    size_t offsetY;
} token_t;

/* prototypes */
token_t *init_token();
void print_token(token_t *token);
void push_char_in_token(token_t *token, char ch);
void push_token_in_queue(queue_t *queue, token_t **token);
