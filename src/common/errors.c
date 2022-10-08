#include "../main.h"
#include "queue.h"
#include "../scanner/token.c"
#include "error.h"

program_t program;

/**
 * @brief Program clean up function
 */
void free_program(){
    free_queue(program.queue, free_token);
}

/**
 * @brief Error Handling function
 *  
 * @param error_code Code of the error
 */
void exit_failure(int error_code)
{
    free_program();
    fprintf(stderr, "ERROR: ");
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
    }
    exit(error_code);
}