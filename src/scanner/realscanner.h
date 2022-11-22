
#ifndef SCANNER
#define SCANNER
#include "input_stack.h"
#include "../common/logging.h"

declare_logging(scanner)

typedef struct scanner_t
{
    token_t *current_token;
    token_t* (*get_next_token)(struct scanner_t *self);
    bool white_space_reading;
    struct scanner_t* (*init_scanner)();
    void (*free)(struct scanner_t *self);
    scanner_logging_t *logger;
} scanner_t;

scanner_t* init_scanner();
void destruct_scanner_t(scanner_t *self);

#include "../common/error.h"

#endif
