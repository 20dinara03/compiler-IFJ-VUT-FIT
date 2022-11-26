#ifndef CODE_TEMPLATES_H
#define CODE_TEMPLATES_H

#include <stdlib.h>
#include <stdio.h>
#include "code_generation_base.h"

typedef enum {
    LF,
    TF,
    GF,
    INT,
    FLOAT,
    STRING,
    BOOL,
    NIL,
} arg_type;

typedef struct arg_t {
    char *name;
    void (*free)(struct arg_t *self);
} arg_t;
arg_t *new_arg(arg_type frame, char *name);
void free_arg(arg_t *self);

typedef struct label_t {
    char *name;
    void (*free)(struct label_t *self);
} label_t;
label_t *new_label(code_stack_t *node, code_type name);
void free_label(label_t *self);

typedef struct code_templater_t {
    // 10.4.1
    char* (*DEFVAR)(arg_t *var);
    char* (*MOVE)(arg_t *var, arg_t *symb);
    char* (*CREATEFRAME)();
    char* (*PUSHFRAME)();
    char* (*POPFRAME)();
    char* (*CALL)(label_t *label);
    char* (*RETURN)();

    // 10.4.2
    char* (*PUSHS)(arg_t *symb);
    char* (*POPS)(arg_t *var);
    char* (*CLEARS)();

    // 10.4.3
    char* (*ADD)(arg_t *var, arg_t *symb1, arg_t *symb2);
    char* (*SUB)(arg_t *var, arg_t *symb1, arg_t *symb2);
    char* (*MUL)(arg_t *var, arg_t *symb1, arg_t *symb2);
    char* (*DIV)(arg_t *var, arg_t *symb1, arg_t *symb2);
    char* (*IDIV)(arg_t *var, arg_t *symb1, arg_t *symb2);
    char* (*ADDS)(arg_t *var, arg_t *symb1, arg_t *symb2);
    char* (*SUBS)(arg_t *var, arg_t *symb1, arg_t *symb2);
    char* (*MULS)(arg_t *var, arg_t *symb1, arg_t *symb2);
    char* (*DIVS)(arg_t *var, arg_t *symb1, arg_t *symb2);
    char* (*IDIVS)(arg_t *var, arg_t *symb1, arg_t *symb2);
    char* (*LT)(arg_t *var, arg_t *symb1, arg_t *symb2);
    char* (*GT)(arg_t *var, arg_t *symb1, arg_t *symb2);
    char* (*EQ)(arg_t *var, arg_t *symb1, arg_t *symb2);
    char* (*LTS)(arg_t *var, arg_t *symb1, arg_t *symb2);
    char* (*GTS)(arg_t *var, arg_t *symb1, arg_t *symb2);
    char* (*EQS)(arg_t *var, arg_t *symb1, arg_t *symb2);
    char* (*AND)(arg_t *var, arg_t *symb1, arg_t *symb2);
    char* (*OR)(arg_t *var, arg_t *symb1, arg_t *symb2);
    char* (*NOT)(arg_t *var, arg_t *symb1, arg_t *symb2);
    char* (*ANDS)(arg_t *var, arg_t *symb1, arg_t *symb2);
    char* (*ORS)(arg_t *var, arg_t *symb1, arg_t *symb2);
    char* (*NOTS)(arg_t *var, arg_t *symb1, arg_t *symb2);
    char* (*INT2FLOAT)(arg_t *var, arg_t *symb);
    char* (*FLOAT2INT)(arg_t *var, arg_t *symb);
    char* (*INT2CHAR)(arg_t *var, arg_t *symb);
    char* (*STRI2INT)(arg_t *var, arg_t *symb1, arg_t *symb2);
    char* (*INT2FLOATS)(arg_t *var, arg_t *symb);
    char* (*FLOAT2INTS)(arg_t *var, arg_t *symb);
    char* (*INT2CHARS)(arg_t *var, arg_t *symb);
    char* (*STRI2INTS)(arg_t *var, arg_t *symb1, arg_t *symb2);

    // 10.4.4
    char* (*READ)(arg_t *var, arg_t *type);
    char* (*WRITE)(arg_t *symb);

    // 10.4.5
    char* (*CONCAT)(arg_t *var, arg_t *symb1, arg_t *symb2);
    char* (*STRLEN)(arg_t *var, arg_t *symb);
    char* (*GETCHAR)(arg_t *var, arg_t *symb1, arg_t *symb2);
    char* (*SETCHAR)(arg_t *var, arg_t *symb1, arg_t *symb2);

    // 10.4.6
    char* (*TYPE)(arg_t *var, arg_t *symb);

    // 10.4.7
    char* (*LABEL)(label_t *label);
    char* (*JUMP)(label_t *label);
    char* (*JUMPIFEQ)(label_t *label, arg_t *var1, arg_t *var2);
    char* (*JUMPIFNEQ)(label_t *label, arg_t *var1, arg_t *var2);
    char* (*JUMPIFEQS)(label_t *label);
    char* (*JUMPIFNEQS)(label_t *label);
    char* (*EXIT)(char *var); // range 0-49 else err57

    void (*free)(struct code_templater_t *self);
} code_templater_t;

code_templater_t* new_code_templater();
void free_code_templater(code_templater_t *templater);


#define MAX_CODE_LINE_LENGTH 256

// 10.4.1 FRAMES
#define CODE_FRAME_FUNCTION_WITH_2_ARGS_DEC(_name) char* code_##_name(arg_t *var, arg_t *symbol);
#define CODE_FRAME_FUNCTION_WITH_2_ARGS(_name) char* code_##_name(arg_t *var, arg_t *symbol) {    \
    char *code_line = (char*)malloc(sizeof(char) * MAX_CODE_LINE_LENGTH);                       \
    sprintf(code_line, "%s %s %s", #_name, var->name, symbol->name);                             \
    var->free(var);                                                                             \
    symbol->free(symbol);                                                                       \
    return code_line;                                                                           \
}

#define CODE_FRAME_FUNCTION_WITH_1_ARG_DEC(_name) char* code_##_name(arg_t *var);
#define CODE_FRAME_FUNCTION_WITH_1_ARG(_name) char* code_##_name(arg_t *var) {                    \
    char *code_line = (char*)malloc(sizeof(char) * MAX_CODE_LINE_LENGTH);                       \
    sprintf(code_line, "%s %s\n", #_name, var->name);                                              \
    var->free(var);                                                                             \
    return code_line;                                                                           \
}

#define CODE_FRAME_FUNCTION_WITH_1_ARG_LABEL_DEC(_name) char* code_##_name(label_t *label);
#define CODE_FRAME_FUNCTION_WITH_1_ARG_LABEL(_name) char* code_##_name(label_t *label) {          \
    char *code_line = (char*)malloc(sizeof(char) * MAX_CODE_LINE_LENGTH);                       \
    sprintf(code_line, "%s %s\n", #_name, label->name);                                            \
    label->free(label);                                                                         \
    return code_line;                                                                           \
}

#define CODE_FRAME_FUNCTION_DEC(_name) char* code_##_name();
#define CODE_FRAME_FUNCTION(_name) char* code_##_name() {                                         \
    char *code_line = (char*)malloc(sizeof(char) * MAX_CODE_LINE_LENGTH);                       \
    sprintf(code_line, "%s\n", #_name);                                                            \
    return code_line;                                                                           \
}

// 10.4.2 STACK
#define CODE_STACK_FUNCTION_WITH_1_ARG_DEC(_name) char* code_##_name(arg_t *var);
#define CODE_STACK_FUNCTION_WITH_1_ARG(_name) char* code_##_name(arg_t *var) {                    \
    char *code_line = (char*)malloc(sizeof(char) * MAX_CODE_LINE_LENGTH);                       \
    sprintf(code_line, "%s %s\n", #_name, var->name);                                              \
    var->free(var);                                                                             \
    return code_line;                                                                           \
}

#define CODE_STACK_FUNCTION_DEC(_name) char* code_##_name();
#define CODE_STACK_FUNCTION(_name) char* code_##_name() {                                         \
    char *code_line = (char*)malloc(sizeof(char) * MAX_CODE_LINE_LENGTH);                       \
    sprintf(code_line, "%s\n", #_name);                                                            \
    return code_line;                                                                           \
}

// 10.4.3 ARITHMETIC
#define CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS_DEC(_name) char* code_##_name(arg_t *var1, arg_t  *var2, arg_t  *var3);
#define CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS(_name) char* code_##_name(arg_t *var1, arg_t  *var2, arg_t  *var3) { \
    char *code_line = (char*)malloc(sizeof(char) * MAX_CODE_LINE_LENGTH);                                       \
    sprintf(code_line, "%s %s %s %s\n", #_name, var1->name, var2->name, var3->name);                               \
    var1->free(var1);                                                                                           \
    var2->free(var2);                                                                                           \
    var3->free(var3);                                                                                           \
    return code_line;                                                                                           \
}

#define CODE_ARITHMETIC_FUNCTION_WITH_2_ARGS_DEC(_name) char* code_##_name(arg_t *var1, arg_t  *var2);
#define CODE_ARITHMETIC_FUNCTION_WITH_2_ARGS(_name) char* code_##_name(arg_t *var1, arg_t  *var2) {   \
    char *code_line = (char*)malloc(sizeof(char) * MAX_CODE_LINE_LENGTH);                           \
    sprintf(code_line, "%s %s %s\n", #_name, var1->name, var2->name);                                  \
    var1->free(var1);                                                                               \
    var2->free(var2);                                                                               \
    return code_line;                                                                               \
}

// 10.4.4 IO
#define CODE_IO_FUNCTION_WITH_2_ARGS_DEC(_name) char* code_##_name(arg_t *var1, arg_t  *var2);
#define CODE_IO_FUNCTION_WITH_2_ARGS(_name) char* code_##_name(arg_t *var1, arg_t  *var2) {           \
    char *code_line = (char*)malloc(sizeof(char) * MAX_CODE_LINE_LENGTH);                           \
    sprintf(code_line, "%s %s %s\n", #_name, var1->name, var2->name);                                  \
    var1->free(var1);                                                                               \
    var2->free(var2);                                                                               \
    return code_line;                                                                               \
}

#define CODE_IO_FUNCTION_WITH_1_ARG_DEC(_name) char* code_##_name(arg_t *var);
#define CODE_IO_FUNCTION_WITH_1_ARG(_name) char* code_##_name(arg_t *var) {                           \
    char *code_line = (char*)malloc(sizeof(char) * MAX_CODE_LINE_LENGTH);                           \
    sprintf(code_line, "%s %s\n", #_name, var->name);                                                  \
    var->free(var);                                                                                 \
    return code_line;                                                                               \
}

// 10.4.5 STRING
#define CODE_STRING_FUNCTION_WITH_3_ARGS_DEC(_name) char* code_##_name(arg_t *var1, arg_t  *var2, arg_t  *var3);
#define CODE_STRING_FUNCTION_WITH_3_ARGS(_name) char* code_##_name(arg_t *var1, arg_t  *var2, arg_t  *var3) { \
    char *code_line = (char*)malloc(sizeof(char) * MAX_CODE_LINE_LENGTH);                                   \
    sprintf(code_line, "%s %s %s %s\n", #_name, var1->name, var2->name, var3->name);                           \
    var1->free(var1);                                                                                       \
    var2->free(var2);                                                                                       \
    var3->free(var3);                                                                                       \
    return code_line;                                                                                       \
}

#define CODE_STRING_FUNCTION_WITH_2_ARGS_DEC(_name) char* code_##_name(arg_t *var1, arg_t  *var2);
#define CODE_STRING_FUNCTION_WITH_2_ARGS(_name) char* code_##_name(arg_t *var1, arg_t  *var2) {   \
    char *code_line = (char*)malloc(sizeof(char) * MAX_CODE_LINE_LENGTH);                       \
    sprintf(code_line, "%s %s %s\n", #_name, var1->name, var2->name);                              \
    var1->free(var1);                                                                           \
    var2->free(var2);                                                                           \
    return code_line;                                                                           \
}

// 10.4.6 TYPE
#define CODE_TYPE_FUNCTION_WITH_2_ARGS_DEC(_name) char* code_##_name(arg_t *var1, arg_t  *var2);
#define CODE_TYPE_FUNCTION_WITH_2_ARGS(_name) char* code_##_name(arg_t *var1, arg_t  *var2) {     \
    char *code_line = (char*)malloc(sizeof(char) * MAX_CODE_LINE_LENGTH);                       \
    sprintf(code_line, "%s %s %s\n", #_name, var1->name, var2->name);                              \
    var1->free(var1);                                                                           \
    var2->free(var2);                                                                           \
    return code_line;                                                                           \
}

// 10.4.7 PIPELINES
// TODO: excplicit EXIT func
#define CODE_PIPELINE_FUNCTION_DEC(_name) char* code_##_name(label_t *label);
#define CODE_PIPELINE_FUNCTION(_name)                                                            \
    char* code_##_name(label_t *label) {                                                           \
        char *code = (char*)malloc(sizeof(char) * MAX_CODE_LINE_LENGTH);                        \
        sprintf(code, "%s .%s\n", #_name, label->name);                                            \
        label->free(label);                                                                     \
        return code;                                                                            \
    }

#define CODE_PIPELINE_FUNCTION_WITH_2_ARG_DEC(_name) char* code_##_name(label_t *label, arg_t  *var1, arg_t  *var2);
#define CODE_PIPELINE_FUNCTION_WITH_2_ARG(_name) \
    char* code_##_name(label_t *label, arg_t  *var1, arg_t  *var2) {                               \
        char *code = (char*)malloc(sizeof(char) * MAX_CODE_LINE_LENGTH);                        \
        sprintf(code, "%s .%s %s %s\n", #_name, label->name, var1->name, var2->name);              \
        var1->free(var1);                                                                       \
        var2->free(var2);                                                                       \
        label->free(label);                                                                     \
        return code;                                                                            \
    }

// 10.4.1 FRAMES
CODE_FRAME_FUNCTION_DEC(CREATEFRAME)
CODE_FRAME_FUNCTION_DEC(PUSHFRAME)
CODE_FRAME_FUNCTION_DEC(POPFRAME)
CODE_FRAME_FUNCTION_DEC(RETURN)
CODE_FRAME_FUNCTION_WITH_1_ARG_DEC(DEFVAR)
CODE_FRAME_FUNCTION_WITH_1_ARG_LABEL_DEC(CALL)
CODE_FRAME_FUNCTION_WITH_2_ARGS_DEC(MOVE)

// 10.4.2 STACK
CODE_STACK_FUNCTION_DEC(CLEARS)
CODE_STACK_FUNCTION_WITH_1_ARG_DEC(PUSHS)
CODE_STACK_FUNCTION_WITH_1_ARG_DEC(POPS)

// 10.4.3 ARITHMETIC
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS_DEC(ADD)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS_DEC(SUB)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS_DEC(MUL)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS_DEC(DIV)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS_DEC(IDIV)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS_DEC(ADDS)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS_DEC(SUBS)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS_DEC(MULS)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS_DEC(DIVS)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS_DEC(IDIVS)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS_DEC(LT)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS_DEC(GT)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS_DEC(EQ)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS_DEC(LTS)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS_DEC(GTS)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS_DEC(EQS)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS_DEC(AND)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS_DEC(OR)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS_DEC(NOT)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS_DEC(ANDS)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS_DEC(ORS)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS_DEC(NOTS)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS_DEC(STRI2INT)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS_DEC(STRI2INTS)
CODE_ARITHMETIC_FUNCTION_WITH_2_ARGS_DEC(INT2FLOAT)
CODE_ARITHMETIC_FUNCTION_WITH_2_ARGS_DEC(FLOAT2INT)
CODE_ARITHMETIC_FUNCTION_WITH_2_ARGS_DEC(INT2CHAR)
CODE_ARITHMETIC_FUNCTION_WITH_2_ARGS_DEC(FLOAT2INTS)
CODE_ARITHMETIC_FUNCTION_WITH_2_ARGS_DEC(INT2CHARS)
CODE_ARITHMETIC_FUNCTION_WITH_2_ARGS_DEC(INT2FLOATS)

// 10.4.4 I/O
CODE_IO_FUNCTION_WITH_2_ARGS_DEC(READ)
CODE_IO_FUNCTION_WITH_1_ARG_DEC(WRITE)

// 10.4.5 STRING
CODE_STRING_FUNCTION_WITH_3_ARGS_DEC(CONCAT)
CODE_STRING_FUNCTION_WITH_3_ARGS_DEC(GETCHAR)
CODE_STRING_FUNCTION_WITH_3_ARGS_DEC(SETCHAR)
CODE_STRING_FUNCTION_WITH_2_ARGS_DEC(STRLEN)

// 10.4.6 TYPE
CODE_TYPE_FUNCTION_WITH_2_ARGS_DEC(TYPE)

// 10.4.7 PIPELINES
CODE_PIPELINE_FUNCTION_DEC(LABEL)
CODE_PIPELINE_FUNCTION_DEC(JUMP)
CODE_PIPELINE_FUNCTION_DEC(JUMPIFEQS)
CODE_PIPELINE_FUNCTION_DEC(JUMPIFNEQS)
CODE_PIPELINE_FUNCTION_DEC(EXIT)
CODE_PIPELINE_FUNCTION_WITH_2_ARG_DEC(JUMPIFEQ)
CODE_PIPELINE_FUNCTION_WITH_2_ARG_DEC(JUMPIFNEQ)

#endif //CODE_TEMPLATES_H
