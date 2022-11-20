#ifndef INPUT_STACK
#define INPUT_STACK

#include "token.h"

typedef struct input_node_t input_node_t;
typedef struct input_stack_t input_stack_t;

/* list node struct */
struct input_node_t
{
    token_t *token;
    input_node_t *next;
};

/* stack struct */
struct input_stack_t
{
    /*head of the input stack*/
    input_node_t *head;
    /*length of the input stack*/
    size_t length;
    /**
     * @brief Appends token to a stack
     *
     * @param input_stack_t pointer to a current input_stack_t
     * @param token_t current token
     */
    void (*push)(input_stack_t *, token_t *);
    /**
     * @brief Destructor for the stack
     *
     * @param input_stack_t pointer to a current input_stack_t
     */
    void (*free)(input_stack_t **);
};

/**
 * @brief Constructor for the input_stack_t
 *
 * @param token pointer to a token
 * @return new pointer to input_stack_t
 */
input_node_t *init_input_node(token_t *token);

/**
 * @brief Contructor for the input_node_t
 *
 * @return new pointer to input_node_t
 */
input_stack_t *init_input_stack();

#endif