#include "logging.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Checks if the environment variable `IFJ2022_COMPONENT` is set to some component
 *
 * @param env_var name of the component
 * @return
 */
bool check_env_var(char *env_var) {
    char *env_var_value = getenv("IFJ2022_COMPONENT");
    return (env_var_value != NULL) ? strcmp(env_var_value, env_var) == 0 : false;
}

void log_fatal(char *message, int exit_code) {
    fprintf(stderr, "FATAL: %s", message);
    exit_failure(exit_code);
}
