/*
 * Implementace překladače imperativního jazyka IFJ22
 *
 *      Oleksandr Turytsia  | xturyt00 | team-leader
 *      Mark Barzali        | xbarza00
 *      Dinara Garipova     | xgarip00
 *      Kateryna Zdebska    | xzdebs00
 */
#ifndef PROGRAM_H
#define PROGRAM_H

#include "../scanner/token.h"
#include "../scanner/input_stack.h"
#include "../scanner/realscanner.h"
#include "../parser/parser.h"

/* Program structures structure */
typedef struct program_t {
    scanner_t *scanner;
    parser_t *parser;
    FILE *src;
} program_t;

#endif //PROGRAM_H
