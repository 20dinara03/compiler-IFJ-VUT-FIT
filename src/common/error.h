/*
 * Implementace překladače imperativního jazyka IFJ22
 *
 *      Oleksandr Turytsia  | xturyt00 | team-leader
 *      Mark Barzali        | xbarza00
 *      Dinara Garipova     | xgarip00
 *      Kateryna Zdebska    | xzdebs00
 */
#ifndef ERROR
#define ERROR

void free_program();
void exit_failure(int error_code);

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

#endif
