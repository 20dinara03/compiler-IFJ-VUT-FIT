
#ifndef SCANNER
#define SCANNER
#include "input_stack.h"

// typedef struct scanner;

typedef struct
{
    token_t *current_token;
    token_t* (*get_next_token)();
} scanner_t;


scanner_t* init_scanner();

#include "../common/error.h"

#endif
