/**
 * @brief Save memory allocator
 * 
 * @param target any pointer where memory is allocated
 * @param bytes number of bytes that will be allocated
 */
#define memo_allocate(target, bytes)          \
    do                                       \
    {                                        \
        if ((target = malloc(bytes)) == NULL) \
        {                                    \
            /* error handler */              \
        }                                    \
    } while (0)

/**
 * @brief Save memory reallocator
 * 
 * @param target any pointer where memory is reallocated
 * @param bytes number of bytes that will be reallocated
 */
#define memo_reallocate(target, bytes)                 \
    do                                                \
    {                                                 \
        if ((target = realloc(target, bytes)) == NULL) \
        {                                             \
            /* error handler */                       \
        }                                             \
    } while (0)
