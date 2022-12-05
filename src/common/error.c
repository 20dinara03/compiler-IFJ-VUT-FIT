/*
 * Implementace překladače imperativního jazyka IFJ22
 *
 *      Oleksandr Turytsia  | xturyt00 | team-leader
 *      Mark Barzali        | xbarza00
 *      Dinara Garipova     | xgarip00
 *      Kateryna Zdebska    | xzdebs00
 */
#include "error.h"
#include "../main.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Program clean up function
 */
void free_program(){
    program.parser->free(program.parser);
}

/**
 * @brief Error Handling function
 *  
 * @param error_code Code of the error
 */
void exit_failure(int error_code)
{
    free_program();
    fprintf(stderr, "ERROR:");
    switch (error_code)
    {
        case LEXICAL_ANALYSIS_ERR:
            fprintf(stderr, "Incorrect structure of the current lexeme.");
            break;
        case SYNTAXIS_ANALYSIS_ERR:
            fprintf(stderr, "Incorrect program syntax, missing header, etc.");
            break;
        case SEMANTIC_UNDEFINED_FUNC_ERR:
            fprintf(stderr, "Undefined function, attempt to redefine the function.");
            break;
        case SEM_RUN_ARGS_OR_RETURN_FUNC_ERR:
            fprintf(stderr, "Wrong number/type of parameters when calling the function or the type of the return value from the function.");
            break;
        case SEMANTIC_UNDEFINED_VAR_ERR:
            fprintf(stderr, "Use of an undefined variable.");
            break;
        case SEM_RUN_MISS_RETURN_FUNC_ERR:
            fprintf(stderr, "Missing/remaining expression in the return command from the function.");
            break;
        case SEM_RUN_TYPE_COMPAT_IN_EXPR_ERR:
            fprintf(stderr, "Type compatibility error in arithmetic, string an relational expressions.");
            break;
        case SEMANIC_OTHER_ERR:
            fprintf(stderr, "Other semantic errors.");
            break;
        case INTERNAL_COMPILER_ERR:
            fprintf(stderr, "Internal compiler error.");
            break;
        default:
            fprintf(stderr, "Unknown error.");
            break;
    }
    exit(error_code);
}
