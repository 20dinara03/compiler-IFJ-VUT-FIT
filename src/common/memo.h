/**
 * @brief Save memory allocator
 *
 * @param target any pointer where memory is allocated
 * @param bytes number of bytes that will be allocated
 */
#define memo_allocate(target, type, size)                           \
    do                                                              \
    {                                                               \
        if ((target = (type *)malloc(sizeof(type) * size)) == NULL) \
        {                                                           \
            /* error handler */                                     \
        }                                                           \
    } while (0)

/**
 * @brief Save memory reallocator
 *
 * @param target any pointer where memory is reallocated
 * @param bytes number of bytes that will be reallocated
 */
#define memo_reallocate(target, type, size)                   \
    do                                                        \
    {                                                         \
        if ((target = (type *)realloc(target, size)) == NULL) \
        {                                                     \
            /* error handler */                               \
        }                                                     \
    } while (0)
