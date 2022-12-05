#include <stdio.h>
#include "code_generation_base.h"
#include "code_templates.h"


code_line_t* new_code_line(char* line) {
    code_line_t* self = malloc(sizeof(code_line_t));
    self->line = line;
    self->free = destruct_code_line;
    self->release = code_line_release;
    return self;
}

void destruct_code_line(code_line_t *self) {
    free(self->line);
    self->line = NULL;
    if (self != NULL)
        free(self);
    self = NULL;
}

void code_line_release(code_line_t *self) {
    if (self->line != NULL)
        printf("%s", self->line);
}

code_block_t* new_code_block(int id) {
    code_block_t* self = malloc(sizeof(code_block_t));
    self->id = id;
    self->lines = malloc(sizeof(code_line_t*));
    self->size = 0;
    self->free = destruct_code_block;
    self->release = code_block_release;
    self->add_line = code_block_add_line;
    return self;
}

void destruct_code_block(code_block_t *self) {
    if (self->lines != NULL) {
        for (int i = 0; i < self->size; i++)
            if (self->lines[i] != NULL) {
                self->lines[i]->free(self->lines[i]);
                self->lines[i] = NULL;
            }
        free(self->lines);
        self->lines = NULL;
    }
    free(self);
    self = NULL;
}

void code_block_release(code_block_t *self) {
    if (self->lines != NULL) {
        for (int i = 0; i < self->size; i++) {
            self->lines[i]->release(self->lines[i]);
            self->lines[i]->free(self->lines[i]);
            self->lines[i] = NULL;
        }
        free(self->lines);
    }
    self->lines = NULL;

}

void code_block_add_line(code_block_t *self, char* line) {
    code_line_t *new_line = new_code_line(line);
    if (self->lines == NULL) {
        self->lines = malloc(sizeof(code_line_t*));
        self->size = 0;
    } else
        self->lines = realloc(self->lines, sizeof(code_line_t *) * (self->size + 1));
    self->lines[self->size++] = new_line;
}

code_stack_t* new_code_stack() {
    code_stack_t* self = malloc(sizeof(code_stack_t));
    self->id = 0;
    self->size = 0;
    self->blocks = malloc(sizeof(code_block_t*));
    self->active = true;
    self->templater = new_code_templater();
    self->free = destruct_code_stack;
    self->top = code_stack_top;
    self->push = code_stack_push;
    self->pop = code_stack_pop;
    return self;
}

void destruct_code_stack(code_stack_t *self) {
    for (int i = 0; i < self->size; i++) {
        self->blocks[i]->release(self->blocks[i]);
        self->blocks[i]->free(self->blocks[i]);
    }
    self->templater->free(self->templater);
    free(self->blocks);
    free(self);
}

code_block_t* code_stack_push(code_stack_t *self, bool release) {
    self->blocks = realloc(self->blocks, sizeof(code_block_t*) * (self->size + 1));
    self->blocks[self->size] = new_code_block(++self->id);
    if (self->size > 0 && self->active && release)
        self->blocks[self->size - 1]->release(self->blocks[self->size - 1]);
    return self->blocks[self->size++];
}

void code_stack_pop(code_stack_t *self, bool release) {
    if (release) {
        --self->size;
        if (self->active)
            self->blocks[self->size]->release(self->blocks[self->size]);
        self->blocks[self->size]->free(self->blocks[self->size]);
        self->blocks[self->size] = NULL;
    }
}

code_block_t* code_stack_top(code_stack_t *self) {
    return self->blocks[self->size - 1];
}
