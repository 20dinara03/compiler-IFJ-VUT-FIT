#include "queue.h"

/**
 * @brief Queue clean up function
 *
 * @param queue pointer to queue_t
 * @param free_data clean up function for given data
 */
#define free_queue(queue, free_data)   \
    do                                 \
    {                                  \
        while (queue->head)            \
            dequeue(queue, free_data); \
        free(queue);                   \
    } while (0)

/**
 * @brief Token clean up function
 *
 * @param node pointer to node_t
 * @param free_data clean up function for given data
 */
#define free_node(node, free_data) \
    do                             \
    {                              \
        free_data(node->data);     \
        free(node);                \
    } while (0)

/**
 * @brief removes an item from the head.
 *
 * @param queue pointer to queue_t
 * @param free_data clean up function for given data
 */
#define dequeue(queue, free_data)          \
    do                                     \
    {                                      \
        node_t *next = queue->head->next;  \
        free_node(queue->head, free_data); \
        queue->head = next;                \
        queue->length--;                   \
    } while (0)

/**
 * @brief Inserts data of any type at the end of the queue
 *
 * @param queue pointer to queue_t
 * @param data any pointer of any type to the data
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
 * @brief Contructor for the queue_t
 *
 * @return new pointer to queue_t
 */
queue_t *init_queue()
{
    queue_t *queue = NULL;

    memo_allocate(queue, sizeof(queue_t));

    return queue;
}

/**
 * @brief Contructor for the node_t
 *
 * @return new pointer to node_t
 */
node_t *init_node(void *data)
{
    node_t *node = NULL;

    memo_allocate(node, sizeof(node_t));

    node->data = data;

    return node;
}
