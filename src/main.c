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

    //parser demo

    queue_t *queue = init_queue();
    token_t *token = init_token();

    //parsing expression $a + $b * $c

    push_char_in_token(token, '$');
    push_char_in_token(token, 'a');
    token->type = IDENTIFIER;
    push_token_in_queue(queue, &token);

    push_char_in_token(token, '+');
    token->type = OPERATOR;
    push_token_in_queue(queue, &token);

    push_char_in_token(token, '$');
    push_char_in_token(token, 'b');
    token->type = IDENTIFIER;
    push_token_in_queue(queue, &token);

    push_char_in_token(token, '*');
    token->type = OPERATOR;
    push_token_in_queue(queue, &token);

    push_char_in_token(token, '$');
    push_char_in_token(token, 'c');
    token->type = IDENTIFIER;
    push_token_in_queue(queue, &token);

    push_char_in_token(token, ';');
    token->type = OPERATOR;
    push_token_in_queue(queue, &token);

    node_t*node = queue->head;

    while(node){
        print_token(node->data);
        node = node->next;
    }

    free_token(token);
    free_queue(queue, free_token);

    return 0;
}
