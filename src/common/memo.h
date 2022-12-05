/*
 * Implementace překladače imperativního jazyka IFJ22
 *
 *      Oleksandr Turytsia  | xturyt00 | team-leader
 *      Mark Barzali        | xbarza00
 *      Dinara Garipova     | xgarip00
 *      Kateryna Zdebska    | xzdebs00
 */
#ifndef MEMO
#define MEMO

/**
 * @brief Save memory allocator
 *
 * @param target any pointer where memory is allocated
 * @param bytes number of bytes that will be allocated
 */
#define malloc_s(target, type, size)                                      \
    do                                                                    \
    {                                                                     \
        if ((target = (type *)calloc(size, sizeof(type) * size)) == NULL) \
        {                                                                 \
            /* error handler */                                           \
        }                                                                 \
    } while (0)

/**
 * @brief Save memory reallocator
 *
 * @param target any pointer where memory is reallocated
 * @param bytes number of bytes that will be reallocated
 */
#define realloc_s(target, type, size)                         \
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
#define def_memo_free(type)        \
    void free_##type(type *target) \
    {                              \
        free(target);              \
    }

#endif // MEMO
