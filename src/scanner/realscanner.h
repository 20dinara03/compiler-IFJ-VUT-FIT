
#ifndef SCANNER
#define SCANNER
#include "input_stack.h"

// typedef struct scanner;

typedef struct scanner_t
{
    token_t *current_token;
    token_t* (*get_next_token)();
    struct scanner_t* (*init_scanner)();
    void (*free)();
} scanner_t;

scanner_t* init_scanner();
dec_memo_free(scanner_t);

#include "../common/error.h"

#endif
