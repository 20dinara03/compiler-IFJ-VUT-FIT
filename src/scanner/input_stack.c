#include "input_stack.h"

input_node_t *init_input_node(token_t *token)
{
    input_node_t *node = NULL;

    memo_allocate(node, input_node_t, 1);

    node->token = token;

    return node;
}

void push_token_in_stack(input_stack_t *self, token_t *token)
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

    stack->push = push_token_in_stack;
    stack->free = free_stack;

    return stack;
}