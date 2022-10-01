typedef struct node_t node_t;

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
queue_t *init_queue();
node_t *init_node(void *data);
