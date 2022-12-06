#ifndef CODE_TEMPLATES_H
#define CODE_TEMPLATES_H

#include <stdlib.h>
#include <stdio.h>
#include "code_generation_base.h"
#include "../scanner/token.h"

typedef enum {
    LF,
    TF,
    GF,
    INT,
    FLOAT,
    STRING,
    BOOL,
    NIL,
    VOID,
    BLACKHOLE,
} arg_type;

typedef struct arg_t {
    char *name;
    void (*free)(struct arg_t *self);
} arg_t;
arg_t *new_arg(arg_type frame, char *name);
arg_type translate_token_type(types_t t_type);
arg_t *new_simple_arg(char *name);
void free_arg(arg_t *self);

typedef struct label_t {
    char *name;
    void (*free)(struct label_t *self);
} label_t;
label_t *new_label(code_stack_t *node, code_type name, int number);
label_t *new_simple_label(char *name);
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
    char* (*READ)(arg_t *var, arg_t *type); // TODO: type = int, float, string
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
    char* (*EXIT)(arg_t *var); // range 0-49 else err57

    void (*free)(struct code_templater_t *self);
} code_templater_t;

code_templater_t* new_code_templater();
void free_code_templater(code_templater_t *templater);


#define MAX_CODE_LINE_LENGTH 128

// function with void
#define CODE_FUNCTION_WITHOUT_ARGS_DEC(_name) char* code_##_name();
#define CODE_FUNCTION_WITHOUT_ARGS(_name) \
    char* code_##_name() { \
        char *code = malloc(sizeof(char) * MAX_CODE_LINE_LENGTH); \
        sprintf(code, "%s\n", #_name);      \
        return code;                     \
    }

// function with label
#define CODE_FUNCTION_WITH_LABEL_DEC(_name) char* code_##_name(label_t *label);
#define CODE_FUNCTION_WITH_LABEL(_name) char* code_##_name(label_t *label) {  \
    char *code_line = (char*)malloc(sizeof(char) * MAX_CODE_LINE_LENGTH);     \
    sprintf(code_line, "%s %s\n", #_name, label->name);                       \
    label->free(label);                                                       \
    return code_line;                                                         \
}
// function with arg
#define CODE_FUNCTION_WITH_ARG_DEC(_name) char* code_##_name(arg_t *arg);
#define CODE_FUNCTION_WITH_ARG(_name) char* code_##_name(arg_t *arg) {        \
    char *code_line = (char*)malloc(sizeof(char) * MAX_CODE_LINE_LENGTH);     \
    sprintf(code_line, "%s %s\n", #_name, arg->name);                         \
    arg->free(arg);                                                           \
    return code_line;                                                         \
}
// function with label and 2 args
#define CODE_FUNCTION_WITH_LABEL_AND_2_ARGS_DEC(_name) char* code_##_name(label_t *label, arg_t *arg1, arg_t *arg2);
#define CODE_FUNCTION_WITH_LABEL_AND_2_ARGS(_name) char* code_##_name(label_t *label, arg_t *arg1, arg_t *arg2) { \
    char *code_line = (char*)malloc(sizeof(char) * MAX_CODE_LINE_LENGTH);               \
    sprintf(code_line, "%s %s %s %s\n", #_name, label->name, arg1->name, arg2->name);   \
    label->free(label);                                                                 \
    arg1->free(arg1);                                                                   \
    arg2->free(arg2);                                                                   \
    return code_line;                                                                   \
}
// function with arg and arg
#define CODE_FUNCTION_WITH_2_ARGS_DEC(_name) char* code_##_name(arg_t *arg1, arg_t *arg2);
#define CODE_FUNCTION_WITH_2_ARGS(_name) char* code_##_name(arg_t *arg1, arg_t *arg2) { \
    char *code_line = (char*)malloc(sizeof(char) * MAX_CODE_LINE_LENGTH);               \
    sprintf(code_line, "%s %s %s\n", #_name, arg1->name, arg2->name);                   \
    arg1->free(arg1);                                                                   \
    arg2->free(arg2);                                                                   \
    return code_line;                                                                   \
}
// function with arg and arg and arg
#define CODE_FUNCTION_WITH_3_ARGS_DEC(_name) char* code_##_name(arg_t *arg1, arg_t *arg2, arg_t *arg3);
#define CODE_FUNCTION_WITH_3_ARGS(_name) char* code_##_name(arg_t *arg1, arg_t *arg2, arg_t *arg3) { \
    char *code_line = (char*)malloc(sizeof(char) * MAX_CODE_LINE_LENGTH);               \
    sprintf(code_line, "%s %s %s %s\n", #_name, arg1->name, arg2->name, arg3->name);    \
    arg1->free(arg1);                                                                   \
    arg2->free(arg2);                                                                   \
    arg3->free(arg3);                                                                   \
    return code_line;                                                                   \
}

CODE_FUNCTION_WITHOUT_ARGS_DEC(CREATEFRAME)
CODE_FUNCTION_WITHOUT_ARGS_DEC(PUSHFRAME)
CODE_FUNCTION_WITHOUT_ARGS_DEC(POPFRAME)
CODE_FUNCTION_WITHOUT_ARGS_DEC(RETURN)
CODE_FUNCTION_WITHOUT_ARGS_DEC(CLEARS)
CODE_FUNCTION_WITHOUT_ARGS_DEC(READS)
CODE_FUNCTION_WITHOUT_ARGS_DEC(READI)
CODE_FUNCTION_WITHOUT_ARGS_DEC(READF)

CODE_FUNCTION_WITH_LABEL_DEC(JUMPIFEQS)
CODE_FUNCTION_WITH_LABEL_DEC(JUMPIFNEQS)
CODE_FUNCTION_WITH_LABEL_DEC(CALL)
CODE_FUNCTION_WITH_LABEL_DEC(LABEL)
CODE_FUNCTION_WITH_LABEL_DEC(JUMP)

CODE_FUNCTION_WITH_ARG_DEC(DEFVAR)
CODE_FUNCTION_WITH_ARG_DEC(EXIT)
CODE_FUNCTION_WITH_ARG_DEC(POPS)
CODE_FUNCTION_WITH_ARG_DEC(PUSHS)
CODE_FUNCTION_WITH_ARG_DEC(WRITE)

CODE_FUNCTION_WITH_2_ARGS_DEC(MOVE)
CODE_FUNCTION_WITH_2_ARGS_DEC(INT2FLOAT)
CODE_FUNCTION_WITH_2_ARGS_DEC(FLOAT2INT)
CODE_FUNCTION_WITH_2_ARGS_DEC(INT2CHAR)
CODE_FUNCTION_WITH_2_ARGS_DEC(INT2FLOATS)
CODE_FUNCTION_WITH_2_ARGS_DEC(FLOAT2INTS)
CODE_FUNCTION_WITH_2_ARGS_DEC(INT2CHARS)
CODE_FUNCTION_WITH_2_ARGS_DEC(READ)
CODE_FUNCTION_WITH_2_ARGS_DEC(STRLEN)
CODE_FUNCTION_WITH_2_ARGS_DEC(TYPE)

CODE_FUNCTION_WITH_3_ARGS_DEC(ADD)
CODE_FUNCTION_WITH_3_ARGS_DEC(SUB)
CODE_FUNCTION_WITH_3_ARGS_DEC(MUL)
CODE_FUNCTION_WITH_3_ARGS_DEC(DIV)
CODE_FUNCTION_WITH_3_ARGS_DEC(IDIV)
CODE_FUNCTION_WITH_3_ARGS_DEC(ADDS)
CODE_FUNCTION_WITH_3_ARGS_DEC(SUBS)
CODE_FUNCTION_WITH_3_ARGS_DEC(MULS)
CODE_FUNCTION_WITH_3_ARGS_DEC(DIVS)
CODE_FUNCTION_WITH_3_ARGS_DEC(IDIVS)
CODE_FUNCTION_WITH_3_ARGS_DEC(LT)
CODE_FUNCTION_WITH_3_ARGS_DEC(GT)
CODE_FUNCTION_WITH_3_ARGS_DEC(EQ)
CODE_FUNCTION_WITH_3_ARGS_DEC(LTS)
CODE_FUNCTION_WITH_3_ARGS_DEC(GTS)
CODE_FUNCTION_WITH_3_ARGS_DEC(EQS)
CODE_FUNCTION_WITH_3_ARGS_DEC(AND)
CODE_FUNCTION_WITH_3_ARGS_DEC(OR)
CODE_FUNCTION_WITH_3_ARGS_DEC(NOT)
CODE_FUNCTION_WITH_3_ARGS_DEC(ANDS)
CODE_FUNCTION_WITH_3_ARGS_DEC(ORS)
CODE_FUNCTION_WITH_3_ARGS_DEC(NOTS)
CODE_FUNCTION_WITH_3_ARGS_DEC(STRI2INT)
CODE_FUNCTION_WITH_3_ARGS_DEC(STRI2INTS)
CODE_FUNCTION_WITH_3_ARGS_DEC(CONCAT)
CODE_FUNCTION_WITH_3_ARGS_DEC(GETCHAR)
CODE_FUNCTION_WITH_3_ARGS_DEC(SETCHAR)

CODE_FUNCTION_WITH_LABEL_AND_2_ARGS_DEC(JUMPIFEQ)
CODE_FUNCTION_WITH_LABEL_AND_2_ARGS_DEC(JUMPIFNEQ)

#endif //CODE_TEMPLATES_H
