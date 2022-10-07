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

//     queue_t *queue = init_queue();
//     token_t *token = init_token();

//     /* scanning expression "$a + $b * $c" */

//     push_char_in_token(token, '$'); // inserts '$' in a token
//     push_char_in_token(token, 'a'); // inserts 'a' in a token
//     token->type = IDENTIFIER; // sets up type of a token
//     push_token_in_queue(queue, &token); // pushes token to a queue, and resets the token (creates a new one)

//     push_char_in_token(token, '+');
//     token->type = OPERATOR;
//     push_token_in_queue(queue, &token);

//     push_char_in_token(token, '$');
//     push_char_in_token(token, 'b');
//     token->type = IDENTIFIER;
//     push_token_in_queue(queue, &token);

//     push_char_in_token(token, '*');
//     token->type = OPERATOR;
//     push_token_in_queue(queue, &token);

//     push_char_in_token(token, '$');
//     push_char_in_token(token, 'c');
//     token->type = IDENTIFIER;
//     push_token_in_queue(queue, &token);

//     push_char_in_token(token, ';');
//     token->type = OPERATOR;
//     push_token_in_queue(queue, &token);

//     node_t*node = queue->head;

//     while(node){
//         print_token(node->data);
//         node = node->next;
//     }

//     /* clean up */

//     free_token(token);
//     free_queue(queue, free_token);

//     return 0;
// }
