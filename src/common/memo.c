/**
 * @brief Save memory allocator
 */
#define memo_allocate(target, type)          \
    do                                       \
    {                                        \
        if ((target = malloc(type)) == NULL) \
        {                                    \
            /* error handler */              \
        }                                    \
    } while (0)

/**
 * @brief Save memory reallocator
 */
#define memo_reallocate(target, type)                 \
    do                                                \
    {                                                 \
        if ((target = realloc(target, type)) == NULL) \
        {                                             \
            /* error handler */                       \
        }                                             \
    } while (0)
