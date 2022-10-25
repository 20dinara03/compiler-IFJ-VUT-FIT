#ifndef TOKEN
#define TOKEN

#include "../main.h"

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
    COMMENT_LINE,
    KEYWORD_h,
    KEYWORD_p,
    EXPONENTA
} types_t;

typedef struct input_node_t input_node_t;
typedef struct input_stack_t input_stack_t;
typedef struct token_t token_t;

/* token struct */
struct token_t
{
    string text;
    types_t type;
    size_t offsetX;
    size_t offsetY;
    /**
     * @brief Appends symbol at the end of the token text
     *
     * @param token pointer to a target token_t
     * @param ch input char
     */
    void (*push_char)(token_t *, char);
    /**
     *
     * @brief Resets token (creates new one)
     *
     * @param token pointer to token_t
     */
    void (*reset)(token_t **);
    /**
     *
     * @brief Prints token in console, in (type : value) format
     *
     * @param token pointer to token_t
     */
    void (*debug)(token_t *);
    /**
     * @brief Token clean up function
     *
     * @param token pointer to token_t
     */
    void (*free)(token_t **);
};

/* list node struct */
struct input_node_t
{
    token_t *token;
    input_node_t *next;
};

/* stack struct */
struct input_stack_t
{
    input_node_t *head;
    size_t length;
    /**
     * @brief Appends symbol at the end of the token text
     *
     * @param token pointer to a target token_t
     * @param ch input char
     */
    void (*push)(input_stack_t *, token_t *);
    void (*free)(input_stack_t **);
};

/**
 * @brief Contructor for the input_stack_t
 *
 * @return new pointer to input_stack_t
 */
input_node_t *init_input_node(token_t *data);

/**
 * @brief Contructor for the input_node_t
 *
 * @return new pointer to input_node_t
 */
input_stack_t *init_input_stack();
/**
 * @brief Constructor for token_t
 *
 * @return new pointer to token_t
 */
token_t *init_token();

#endif /* TOKEN */
