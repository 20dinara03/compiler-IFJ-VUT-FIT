#ifndef CODE_GENERATION_BASE_H
#define CODE_GENERATION_BASE_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct code_templater_t code_templater_t;

typedef enum {
    IF,
    ELSE,
    FUNCTION,
    WHILE,
    JUMPIFNEQ,
    LABEL_END,
    NULLPTR,
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
    code_block_t* (*push)(struct code_stack_t *self);
    void (*pop)(struct code_stack_t *self, bool release);
    void (*free)(struct code_stack_t *self);
} code_stack_t;
code_stack_t* new_code_stack();
void destruct_code_stack(code_stack_t *self);
code_block_t* code_stack_push(code_stack_t *self);
void code_stack_pop(code_stack_t *self, bool release);

#endif //CODE_GENERATION_BASE_H
