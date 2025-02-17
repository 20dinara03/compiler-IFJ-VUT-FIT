/*
 * Implementace překladače imperativního jazyka IFJ22
 *
 *      Oleksandr Turytsia  | xturyt00 | team-leader
 *      Mark Barzali        | xbarza00
 *      Dinara Garipova     | xgarip00
 *      Kateryna Zdebska    | xzdebs00
 */
#ifndef CODE_GENERATION_BASE_H
#define CODE_GENERATION_BASE_H

#include <stdlib.h>
#include <stdbool.h>

#define RESULT "%result%"
#define FUN_RETURN "%return%"

typedef struct code_templater_t code_templater_t;

typedef enum {
    IF,
    ELSE,
    IF_END,
    FUNCTION_DEC,
    FUNCTION_CALL,
    WHILE,
    WHILE_END,
    JUMPIFNEQ,
    LABEL_END,
    BUILT_IN,
    BUILT_IN_END,
    NULLPTR,
    EXPR_JMP_1,
    EXPR_JMP_2,
    EXPR_JMP_3,
    EXPR_JMP_4
} code_type;

typedef struct code_line_t {
    char* line;
    void (*free)(struct code_line_t *self);
    void (*release)(struct code_line_t *self);
} code_line_t;
code_line_t* new_code_line(char* line);
void destruct_code_line(code_line_t *self);
void code_line_release(code_line_t *self);

typedef struct code_block_t {
    int id;
    code_line_t** lines;
    int size;
    code_type type;
    void (*release)(struct code_block_t *self);
    void (*free)(struct code_block_t *self);
    void (*add_line)(struct code_block_t *self, char* line);
} code_block_t;
code_block_t* new_code_block(int id);
void destruct_code_block(code_block_t *self);
void code_block_add_line(code_block_t *self, char* line);
void code_block_release(code_block_t *self);

typedef struct code_stack_t {
    int id;
    code_block_t** blocks;
    int size;
    code_templater_t* templater;
    bool active;
    code_block_t* (*top)(struct code_stack_t *self);
    code_block_t* (*push)(struct code_stack_t *self, bool release);
    void (*pop)(struct code_stack_t *self, bool release);
    void (*free)(struct code_stack_t *self);
} code_stack_t;
code_stack_t* new_code_stack();
void destruct_code_stack(code_stack_t *self);
code_block_t* code_stack_push(code_stack_t *self, bool release);
void code_stack_pop(code_stack_t *self, bool release);
code_block_t* code_stack_top(code_stack_t *self);

#endif //CODE_GENERATION_BASE_H
