/*
 * Implementace překladače imperativního jazyka IFJ22
 *
 *      Oleksandr Turytsia  | xturyt00 | team-leader
 *      Mark Barzali        | xbarza00
 *      Dinara Garipova     | xgarip00
 *      Kateryna Zdebska    | xzdebs00
 */
#include "./symbol-variable.h"

#define strcpy_s(dest, src)                     \
    if (src != NULL)                            \
    {                                           \
        if (dest != NULL)                       \
        {                                       \
            realloc_s(dest, char, strlen(src)); \
        }                                       \
        else                                    \
        {                                       \
            malloc_s(dest, char, strlen(src));  \
        }                                       \
        strcpy(dest, src);                      \
    }                                           \
    else                                        \
    {                                           \
        dest = NULL;                            \
    }

/**
 * @brief Creates argument for a function (TF)
 *
 * @param self Self
 * @param name Name of the argument
 * @param value Value of the argument
 * @param type Type of the argument
 */
static void symbol_variable_push_arg(symbol_variable_t *self, string name, string value, arg_type type)
{

    if (self->arg_next == NULL)
    {
        self->arg_next = init_symbol_variable(name, value, type, TF, false);
    }
    else
    {
        symbol_variable_t *var = self->arg_next;
        for (; var->arg_next != NULL; var = var->arg_next)
            ;
        var->arg_next = init_symbol_variable(name, value, type, TF, false);
    }
    self->args_size++;
}

/**
 * @brief Searches for a specific argument by name in a given function
 *
 * @param self self
 * @param name Name of the argument
 * @return symbol_variable_t*
 */
static symbol_variable_t *symbol_variable_find_arg(symbol_variable_t *self, string name)
{
    if (self->is_function == false)
        return NULL;
    for (symbol_variable_t *node = self->arg_next; node != NULL; node = node->arg_next)
    {
        if (strcmp(node->name, name) == 0)
            return node;
    }
    return NULL;
}

static arg_type symbol_variable_find_arg_g(symbol_variable_t *self, string name)
{
    symbol_variable_t *var = symbol_variable_find_arg(self, name);
    if (var == NULL)
        return -1;
    return var->frame;
}

/**
 * @brief Assigns value to a variable
 *
 * @param self Self
 * @param value New value
 * @param type New type (for convertion)
 * @return symbol_table_types
 */
static symbol_table_types symbol_variable_assign(symbol_variable_t *self, string value, arg_type type)
{
    strcpy_s(self->value, value);
    self->type = type;
    return ST_ASSIGN;
}

/**
 * @brief Destructor for the symbol variable
 *
 * @param symbol_node_t pointer to a current symbol_node_t*
 */
static void free_symbol_variable(symbol_variable_t **var)
{
    if ((*var) == NULL)
        return;
    free((*var)->name);
    free((*var)->value);
    for (symbol_variable_t *next = (*var)->arg_next; next != NULL;)
    {
        symbol_variable_t *target = next;
        next = next->arg_next;
        free(target->name);
        free(target->value);
        free(target);
    }
    free((*var));
    (*var) = NULL;
}

/**
 * @brief Creates variable
 *
 * @param name Variable's name
 * @param value Variable's value
 * @param type Variable's type
 * @param frame Frame where variable is located
 * @param is_function True/False if it's a function
 * @return symbol_variable_t*
 */
symbol_variable_t *init_symbol_variable(string name, string value, arg_type type, arg_type frame, bool is_function)
{
    symbol_variable_t *var = NULL;
    malloc_s(var, symbol_variable_t, 1);
    strcpy_s(var->name, name);
    strcpy_s(var->value, value);
    var->type = type;
    var->frame = frame;
    var->is_function = is_function;
    var->arg_next = NULL;
    var->find_arg_g = symbol_variable_find_arg_g;
    var->assign = symbol_variable_assign;
    var->push_arg = symbol_variable_push_arg;
    var->find_arg = symbol_variable_find_arg;
    var->free = free_symbol_variable;
    return var;
}
