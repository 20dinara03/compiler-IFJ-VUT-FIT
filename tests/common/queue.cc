#include <gtest/gtest.h>

extern "C"
{
#include "../../src/common/queue.h"
#include "../../src/scanner/token.h"
}

TEST(Queue, Init)
{
    queue_t *queue = init_queue();

    EXPECT_TRUE(queue->head == queue->tail && queue->head == nullptr && queue->length == 0);
}

TEST(Queue, Enqueue_Easy)
{
    queue_t *queue = init_queue();
    token_t *token = init_token();
    push_char_in_token(token, 'a');
    push_token_in_queue(queue, &token);
    push_char_in_token(token, 'b');
    push_token_in_queue(queue, &token);
    push_char_in_token(token, 'c');
    push_token_in_queue(queue, &token);

    EXPECT_TRUE(queue->length == 3 && strcmp(((token_t *)queue->head->data)->text, "a") == 0 && strcmp(((token_t *)queue->tail->data)->text, "c") == 0);
}

TEST(Queue, Enqueue_Hard)
{
    queue_t *queue = init_queue();
    token_t *token = init_token();
    const int MAX_SIZE = 1000;
    
    char*a = new char[MAX_SIZE+1];
    memset(a,'a',MAX_SIZE);

    for (int i = 0; i < MAX_SIZE; i++)
    {
        for(int j = 0 ;j < i+1;j++){
            push_char_in_token(token, 'a');
        }
        push_token_in_queue(queue, &token);
    }

    EXPECT_TRUE(queue->length == 1000 && strcmp(((token_t *)queue->head->data)->text, "a") == 0 && strcmp(((token_t *)queue->tail->data)->text, a) == 0);
}