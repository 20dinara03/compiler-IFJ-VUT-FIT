// /**
//  * @file token.demo.c
//  * @author xturyt00
//  * @brief Demo usage of queues & tokens for Scanner
//  * @version 0.1
//  */

// #include "../../main.h"
// #include "../../common/queue.c"
// #include "../token.c"

// int main()
// {
//     /* Setting up default values for queues & tokens */

//     input_stack_t *queue = init_input_stack();
//     token_t *token = init_token();

//     /* scanning expression "$a + $b * $c" */

//     push_char(token, '$'); // inserts '$' in a token
//     push_char(token, 'a'); // inserts 'a' in a token
//     token->type = IDENTIFIER; // sets up type of a token
//     push_token_in_queue(queue, &token); // pushes token to a queue, and resets the token (creates a new one)

//     push_char(token, '+');
//     token->type = OPERATOR;
//     push_token_in_queue(queue, &token);

//     push_char(token, '$');
//     push_char(token, 'b');
//     token->type = IDENTIFIER;
//     push_token_in_queue(queue, &token);

//     push_char(token, '*');
//     token->type = OPERATOR;
//     push_token_in_queue(queue, &token);

//     push_char(token, '$');
//     push_char(token, 'c');
//     token->type = IDENTIFIER;
//     push_token_in_queue(queue, &token);

//     push_char(token, ';');
//     token->type = OPERATOR;
//     push_token_in_queue(queue, &token);

//     input_node_t*node = queue->head;

//     while(node){
//         print_token(node->data);
//         node = node->next;
//     }

//     /* clean up */

//     free_token(token);
//     free_queue(queue, free_token);

//     return 0;
// }
