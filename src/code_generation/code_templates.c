#include <string.h>
#include "code_templates.h"

#define register(_name) templater->_name = code_##_name

arg_t* new_arg(arg_type frame, char *name) {
    char* arg_type_transalate[9] = {
        [LF] = "LF",
        [TF] = "TF",
        [GF] = "GF",
        [INT] = "int",
        [STRING] = "string",
        [BOOL] = "bool",
        [NIL] = "nil",
        [FLOAT] = "float",
    };
    arg_t *self = (arg_t*)malloc(sizeof(arg_t));
    self->name = malloc(strlen(name) + 7);
    sprintf(self->name, "%s@%s", arg_type_transalate[frame], name);
    self->free = free_arg;
    return self;
}

void free_arg(arg_t *self) {
    free(self->name);
    free(self);
}

label_t *new_label(code_stack_t *stack, code_type name) {
    char* transalate[] = {
        [IF] = "IF",
        [ELSE] = "ELSE",
        [IF_END] = "IF_END",
        [WHILE] = "WHILE",
        [WHILE_END] = "WHILE_END",
        [FUNCTION_DEC] = "FUNCTION_DEC",
        [FUNCTION_CALL] = "FUNCTION_CALL",
        [LABEL_END] = "END",
        [NULLPTR] = "NULL",
    };
    label_t *self = (label_t*)malloc(sizeof(label_t));
    self->free = free_label;
    self->name = malloc(strlen(transalate[name]) + stack->size * 3);
    sprintf(self->name, "%s", transalate[name]);

    for (int i = 1; i < stack->size; i++) // i = 1 because stack[0] is global
        sprintf(self->name, "%s_%d", transalate[name], stack->blocks[i]->id);

    return self;
}

label_t *new_simple_label(char *name) {
    label_t *self = (label_t*)malloc(sizeof(label_t));
    self->free = free_label;
    self->name = malloc(strlen(name) + 1);
    sprintf(self->name, "%s", name);
    return self;
}

arg_t* new_simple_arg(char* name) {
    arg_t *self = (arg_t*)malloc(sizeof(arg_t));
    self->name = malloc(strlen(name) + 1);
    sprintf(self->name, "%s", name);
    self->free = free_arg;
    return self;
}

void free_label(label_t *self) {
    free(self->name);
    free(self);
}

code_templater_t* new_code_templater() {
    code_templater_t *templater = (code_templater_t*)malloc(sizeof(code_templater_t));

    // 10.4.1
    register(DEFVAR);
    register(MOVE);
    register(CREATEFRAME);
    register(PUSHFRAME);
    register(POPFRAME);
    register(CALL);
    register(RETURN);

    // 10.4.2
    register(PUSHS);
    register(POPS);
    register(CLEARS);

    // 10.4.3
    register(ADD);
    register(SUB);
    register(MUL);
    register(DIV);
    register(IDIV);
    register(ADDS);
    register(SUBS);
    register(MULS);
    register(DIVS);
    register(IDIVS);
    register(LT);
    register(GT);
    register(EQ);
    register(LTS);
    register(GTS);
    register(EQS);
    register(AND);
    register(OR);
    register(NOT);
    register(ANDS);
    register(ORS);
    register(NOTS);
    register(INT2CHAR);
    register(STRI2INT);
    register(INT2CHARS);
    register(STRI2INTS);
    register(INT2FLOAT);
    register(FLOAT2INT);
    register(INT2FLOATS);
    register(FLOAT2INTS);

    // 10.4.4
    register(READ);
    register(WRITE);

    // 10.4.5
    register(CONCAT);
    register(STRLEN);
    register(GETCHAR);
    register(SETCHAR);

    // 10.4.6
    register(TYPE);

    // 10.4.7 PIPELINE
    register(LABEL);
    register(JUMP);
    register(JUMPIFEQ);
    register(JUMPIFNEQ);
    register(JUMPIFEQS);
    register(JUMPIFNEQS);
    register(EXIT);

    templater->free = free_code_templater;
    return templater;
}

void free_code_templater(code_templater_t *templater) {
    free(templater);
}

CODE_FUNCTION_WITHOUT_ARGS(CREATEFRAME)
CODE_FUNCTION_WITHOUT_ARGS(PUSHFRAME)
CODE_FUNCTION_WITHOUT_ARGS(POPFRAME)
CODE_FUNCTION_WITHOUT_ARGS(RETURN)
CODE_FUNCTION_WITHOUT_ARGS(CLEARS)

CODE_FUNCTION_WITH_LABEL(CALL)
CODE_FUNCTION_WITH_LABEL(LABEL)
CODE_FUNCTION_WITH_LABEL(JUMP)
CODE_FUNCTION_WITH_LABEL(JUMPIFEQS)
CODE_FUNCTION_WITH_LABEL(JUMPIFNEQS)

CODE_FUNCTION_WITH_ARG(DEFVAR)
CODE_FUNCTION_WITH_ARG(EXIT)
CODE_FUNCTION_WITH_ARG(POPS)
CODE_FUNCTION_WITH_ARG(PUSHS)
CODE_FUNCTION_WITH_ARG(WRITE)

CODE_FUNCTION_WITH_2_ARGS(READ)
CODE_FUNCTION_WITH_2_ARGS(MOVE)
CODE_FUNCTION_WITH_2_ARGS(STRLEN)
CODE_FUNCTION_WITH_2_ARGS(INT2FLOAT)
CODE_FUNCTION_WITH_2_ARGS(FLOAT2INT)
CODE_FUNCTION_WITH_2_ARGS(INT2CHAR)
CODE_FUNCTION_WITH_3_ARGS(STRI2INT)
CODE_FUNCTION_WITH_2_ARGS(INT2FLOATS)
CODE_FUNCTION_WITH_2_ARGS(FLOAT2INTS)
CODE_FUNCTION_WITH_2_ARGS(INT2CHARS)
CODE_FUNCTION_WITH_3_ARGS(STRI2INTS)
CODE_FUNCTION_WITH_2_ARGS(TYPE)

CODE_FUNCTION_WITH_3_ARGS(ADD)
CODE_FUNCTION_WITH_3_ARGS(SUB)
CODE_FUNCTION_WITH_3_ARGS(MUL)
CODE_FUNCTION_WITH_3_ARGS(DIV)
CODE_FUNCTION_WITH_3_ARGS(IDIV)
CODE_FUNCTION_WITH_3_ARGS(ADDS)
CODE_FUNCTION_WITH_3_ARGS(SUBS)
CODE_FUNCTION_WITH_3_ARGS(MULS)
CODE_FUNCTION_WITH_3_ARGS(DIVS)
CODE_FUNCTION_WITH_3_ARGS(IDIVS)
CODE_FUNCTION_WITH_3_ARGS(LT)
CODE_FUNCTION_WITH_3_ARGS(GT)
CODE_FUNCTION_WITH_3_ARGS(EQ)
CODE_FUNCTION_WITH_3_ARGS(LTS)
CODE_FUNCTION_WITH_3_ARGS(GTS)
CODE_FUNCTION_WITH_3_ARGS(EQS)
CODE_FUNCTION_WITH_3_ARGS(AND)
CODE_FUNCTION_WITH_3_ARGS(OR)
CODE_FUNCTION_WITH_3_ARGS(NOT)
CODE_FUNCTION_WITH_3_ARGS(ANDS)
CODE_FUNCTION_WITH_3_ARGS(ORS)
CODE_FUNCTION_WITH_3_ARGS(NOTS)
CODE_FUNCTION_WITH_3_ARGS(CONCAT)
CODE_FUNCTION_WITH_3_ARGS(GETCHAR)
CODE_FUNCTION_WITH_3_ARGS(SETCHAR)

CODE_FUNCTION_WITH_LABEL_AND_2_ARGS(JUMPIFEQ)
CODE_FUNCTION_WITH_LABEL_AND_2_ARGS(JUMPIFNEQ)

#undef register
