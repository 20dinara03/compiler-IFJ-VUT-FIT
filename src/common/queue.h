#ifndef QUEUE
#define QUEUE

#include "../main.h"

typedef struct node_t node_t;

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
 *
 * @todo Add error handler
 *
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
 *
 * @todo Add error handler instead of break
 *
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

/* list node struct */
struct node_t
{
    void *data;
    node_t *next;
};

/* queue struct */
typedef struct
{
    node_t *head;
    node_t *tail;
    size_t length;
} queue_t;

/* prototypes */
node_t *init_node(void *data);
queue_t *init_queue();

#endif /* QUEUE */