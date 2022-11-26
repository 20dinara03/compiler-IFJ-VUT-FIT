#include <string.h>
#include "code_templates.h"

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
        [WHILE] = "WHILE",
        [FUNCTION] = "FUNCTION",
        [LABEL_END] = "END",
        [NULLPTR] = "NULL",
    };
    label_t *self = (label_t*)malloc(sizeof(label_t));
    self->free = free_label;
    self->name = malloc(strlen(transalate[name]) + stack->size * 3);
    sprintf(self->name, "%s", transalate[name]);

    for (int i = 1; i < stack->size; i++) // i = 1 because stack[0] is global
        sprintf(self->name, "%s_%d", self->name, stack->blocks[i]->id);

    return self;
}

void free_label(label_t *self) {
    free(self->name);
    free(self);
}

code_templater_t* new_code_templater() {
    code_templater_t *templater = (code_templater_t*)malloc(sizeof(code_templater_t));

    // 10.4.1
    templater->DEFVAR = code_DEFVAR;
    templater->MOVE = code_MOVE;
    templater->CREATEFRAME = code_CREATEFRAME;
    templater->PUSHFRAME = code_PUSHFRAME;
    templater->POPFRAME = code_POPFRAME;
    templater->CALL = code_CALL;
    templater->RETURN = code_RETURN;

    // 10.4.2
    templater->PUSHS = code_PUSHS;
    templater->POPS = code_POPS;
    templater->CLEARS = code_CLEARS;

    // 10.4.3
    templater->ADD = code_ADD;
    templater->SUB = code_SUB;
    templater->MUL = code_MUL;
    templater->DIV = code_DIV;
    templater->IDIV = code_IDIV;
    templater->ADDS = code_ADDS;
    templater->SUBS = code_SUBS;
    templater->MULS = code_MULS;
    templater->DIVS = code_DIVS;
    templater->IDIVS = code_IDIVS;
    templater->LT = code_LT;
    templater->GT = code_GT;
    templater->EQ = code_EQ;
    templater->LTS = code_LTS;
    templater->GTS = code_GTS;
    templater->EQS = code_EQS;
    templater->AND = code_AND;
    templater->OR = code_OR;
    templater->NOT = code_NOT;
    templater->ANDS = code_ANDS;
    templater->ORS = code_ORS;
    templater->NOTS = code_NOTS;
    templater->INT2CHAR = code_INT2CHAR;
    templater->STRI2INT = code_STRI2INT;
    templater->INT2CHARS = code_INT2CHARS;
    templater->STRI2INTS = code_STRI2INTS;
    templater->INT2FLOAT = code_INT2FLOAT;
    templater->FLOAT2INT = code_FLOAT2INT;
    templater->INT2FLOATS = code_INT2FLOATS;
    templater->FLOAT2INTS = code_FLOAT2INTS;

    // 10.4.4
    templater->READ = code_READ;
    templater->WRITE = code_WRITE;

    // 10.4.5
    templater->CONCAT = code_CONCAT;
    templater->STRLEN = code_STRLEN;
    templater->GETCHAR = code_GETCHAR;
    templater->SETCHAR = code_SETCHAR;

    // 10.4.6
    templater->TYPE = code_TYPE;

    // 10.4.7 PIPELINE
    templater->LABEL = code_LABEL;
    templater->JUMP = code_JUMP;
    templater->JUMPIFEQ = code_JUMPIFEQ;
    templater->JUMPIFNEQ = code_JUMPIFNEQ;
    templater->JUMPIFEQS = code_JUMPIFEQS;
    templater->JUMPIFNEQS = code_JUMPIFNEQS;
//  TODO:  templater->EXIT = code_EXIT;

    templater->free = free_code_templater;
    return templater;
}

void free_code_templater(code_templater_t *templater) {
    free(templater);
}

// 10.4.1 FRAMES
CODE_FRAME_FUNCTION(CREATEFRAME)
CODE_FRAME_FUNCTION(PUSHFRAME)
CODE_FRAME_FUNCTION(POPFRAME)
CODE_FRAME_FUNCTION(RETURN)
CODE_FRAME_FUNCTION_WITH_1_ARG(DEFVAR)
CODE_FRAME_FUNCTION_WITH_1_ARG_LABEL(CALL)
CODE_FRAME_FUNCTION_WITH_2_ARGS(MOVE)

// 10.4.2 STACK
CODE_STACK_FUNCTION(CLEARS)
CODE_STACK_FUNCTION_WITH_1_ARG(PUSHS)
CODE_STACK_FUNCTION_WITH_1_ARG(POPS)

// 10.4.3 ARITHMETIC
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS(ADD)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS(SUB)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS(MUL)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS(DIV)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS(IDIV)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS(ADDS)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS(SUBS)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS(MULS)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS(DIVS)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS(IDIVS)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS(LT)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS(GT)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS(EQ)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS(LTS)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS(GTS)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS(EQS)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS(AND)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS(OR)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS(NOT)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS(ANDS)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS(ORS)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS(NOTS)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS(STRI2INT)
CODE_ARITHMETIC_FUNCTION_WITH_3_ARGS(STRI2INTS)
CODE_ARITHMETIC_FUNCTION_WITH_2_ARGS(INT2FLOAT)
CODE_ARITHMETIC_FUNCTION_WITH_2_ARGS(FLOAT2INT)
CODE_ARITHMETIC_FUNCTION_WITH_2_ARGS(INT2CHAR)
CODE_ARITHMETIC_FUNCTION_WITH_2_ARGS(FLOAT2INTS)
CODE_ARITHMETIC_FUNCTION_WITH_2_ARGS(INT2CHARS)
CODE_ARITHMETIC_FUNCTION_WITH_2_ARGS(INT2FLOATS)

// 10.4.4 I/O
CODE_IO_FUNCTION_WITH_2_ARGS(READ)
CODE_IO_FUNCTION_WITH_1_ARG(WRITE)

// 10.4.5 STRING
CODE_STRING_FUNCTION_WITH_3_ARGS(CONCAT)
CODE_STRING_FUNCTION_WITH_3_ARGS(GETCHAR)
CODE_STRING_FUNCTION_WITH_3_ARGS(SETCHAR)
CODE_STRING_FUNCTION_WITH_2_ARGS(STRLEN)

// 10.4.6 TYPE
CODE_TYPE_FUNCTION_WITH_2_ARGS(TYPE)

// 10.4.7 PIPELINE
char *code_LABEL(label_t *label) {
    char *code = (char *) malloc(sizeof(char) * 50);
    sprintf(code, "LABEL .%s\n", label->name);
    label->free(label);
    return code;
}
CODE_PIPELINE_FUNCTION(JUMP)
CODE_PIPELINE_FUNCTION(JUMPIFEQS)
CODE_PIPELINE_FUNCTION(JUMPIFNEQS)
CODE_PIPELINE_FUNCTION(EXIT)

CODE_PIPELINE_FUNCTION_WITH_2_ARG(JUMPIFEQ)
CODE_PIPELINE_FUNCTION_WITH_2_ARG(JUMPIFNEQ)
