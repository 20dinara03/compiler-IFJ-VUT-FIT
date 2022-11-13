#ifndef MEMO
#define MEMO

/**
 * @brief Save memory allocator
 *
 * @param target any pointer where memory is allocated
 * @param bytes number of bytes that will be allocated
 */
#define memo_allocate(target, type, size)                           \
    do                                                              \
    {                                                               \
        if ((target = (type *)calloc(size, sizeof(type) * size)) == NULL) \
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

/**
 * @brief macro declaration for freeing memory
 *
 * @param type type of the pointer
 */
#define dec_memo_free(type) \
    void free_##type(type *target);

/**
 * @brief macro creates function to free memory
 *
 * @param type type of pointer
 */
#define def_memo_free(type)  \
    void free_##type(type *target) \
    {                                \
        free(target);                \
    }

#endif //MEMO
