/*
 * Welcome to IFJ/IAL 2022/2023 project
 *
 * authors:
 *      Oleksandr Turytsia  | xturyt00 | team-leader
 *      Mark Barzali        | xbarza00
 *      Dinara Garipova     | xgarip00
 *      Kateryna Zdebska    | xzdebs00
 */
#include "main.h"
#include "./common/queue.c"
#include "./scanner/token.c"

int main()
{
    queue_t *queue = init_queue();
    token_t* token = init_token();

    // string s = "myVariable + b2 = c";

    push_char_in_token(token,'$');
    push_char_in_token(token,'a');
    token->type = IDENTIFIER;
    push_token_in_queue(queue, &token);

    push_char_in_token(token,'+');
    token->type = OPERATOR;
    push_token_in_queue(queue, &token);

    push_char_in_token(token,'$');
    push_char_in_token(token,'b');
    token->type = IDENTIFIER;
    push_token_in_queue(queue, &token);

     push_char_in_token(token,'*');
    token->type = OPERATOR;
    push_token_in_queue(queue, &token);

    push_char_in_token(token,'$');
    push_char_in_token(token,'c');
    token->type = IDENTIFIER;
    push_token_in_queue(queue, &token);

    

    node_t* node = queue->head;
    for (size_t i = 0; i < queue->length; i++)
    {
        print_token(((token_t*)node->data));
        node = node->next;
    }
    

    return 0;
}
