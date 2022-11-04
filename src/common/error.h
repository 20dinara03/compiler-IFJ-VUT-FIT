#include "../scanner/token.h"
#include "../scanner/input_stack.h"

enum ERRORS
{
    LEXICAL_ANALYSIS_ERR = 1,
    SYNTAXIS_ANALYSIS_ERR,
    SEMANTIC_UNDEFINED_FUNC_ERR,
    SEM_RUN_ARGS_OR_RETURN_FUNC_ERR,
    SEMANTIC_UNDEFINED_VAR_ERR,
    SEM_RUN_MISS_RETURN_FUNC_ERR,
    SEM_RUN_TYPE_COMPAT_IN_EXPR_ERR,
    SEMANIC_OTHER_ERR,
    INTERNAL_COMPILER_ERR = 99
};

/* Program structures structure */
typedef struct{
   input_stack_t *queue;
   FILE* src;
} program_t;

program_t program;