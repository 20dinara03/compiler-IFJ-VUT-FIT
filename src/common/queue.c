#include "queue.h"

/**
 * @brief Contructor for the queue_t
 *
 * @return new pointer to queue_t
 */
queue_t *init_queue()
{
    queue_t *queue = NULL;

    memo_allocate(queue, queue_t, 1);

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

    memo_allocate(node, node_t, 1);

    node->data = data;

    return node;
}
