//
// Created by zorgaur on 11/12/22.
//

#ifndef PROGRAM_H
#define PROGRAM_H

#include "../scanner/token.h"
#include "../scanner/input_stack.h"
#include "../scanner/realscanner.h"

/* Program structures structure */
typedef struct program_t {
    input_stack_t *queue;
    scanner_t *scanner;
    FILE *src;
} program_t;

program_t program;

#endif //PROGRAM_H
