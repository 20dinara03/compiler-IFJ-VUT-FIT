#include "queue.h"

/**
 * @brief removes an item from the head. 
 *
 * @param queue current queue
 * @param func function clean up for given data
 */
#define dequeue(queue, func)               \
    do                                     \
    {                                      \
        func(((void *)queue->head->data)); \
        void *temp_head = queue->head;     \
        queue->head = queue->head->next;   \
        queue->length--;                   \
        free(temp_head);                   \
    } while (0)

/**
 * @brief Inserts an item at the end of the queue
 *
 * @param queue current queue
 * @param data 
 */
#define enqueue(queue, data)                             \
    do                                                   \
    {                                                    \
        if (queue == NULL)                               \
            break;                                       \
        if (queue->head == NULL)                         \
        {                                                \
            queue->head = queue->tail = init_node(data); \
        }                                                \
        else                                             \
        {                                                \
            queue->tail->next = init_node(data);         \
            queue->tail = queue->tail->next;             \
        }                                                \
        queue->length++;                                 \
    } while (0)

/**
 * @brief Contructor for the queue
 * 
 * @return queue_t* 
 */
queue_t *init_queue()
{
    queue_t *queue = NULL;

    memo_allocate(queue, sizeof(queue_t));

    return queue;
}

/**
 * @brief Contructor for the node
 * 
 * @return queue_t* 
 */
node_t *init_node(void *data)
{
    node_t *node = NULL;

    memo_allocate(node, sizeof(node_t));

    node->data = data;

    return node;
}
