#ifndef SYMBOL_VARIABLE_H
#define SYMBOL_VARIABLE_H
#include "../scanner/token.h"
#include "../code_generation/code_templates.h"
#include "string.h"
typedef struct symbol_variable_t symbol_variable_t;

/*Symbol tables's code constants*/
typedef enum symbol_table_types
{
    ST_INSERT = 0,
    ST_ASSIGN,
    ST_ERR
} symbol_table_types;

struct symbol_variable_t
{
    /*Variable's name*/
    string name;
    /*Variable's value*/
    string value;
    /*Variable's type*/
    arg_type type;
    arg_type frame;
    /*Function properties*/
    size_t args_size;
    symbol_variable_t *arg_next;
    bool is_function;

    symbol_variable_t *(*find_arg)(symbol_variable_t *self, string name);
    arg_type (*find_arg_g)(symbol_variable_t *self, string name);
    symbol_table_types (*assign)(symbol_variable_t *self, string value, arg_type type);
    void (*push_arg)(symbol_variable_t *self, string name, string value, arg_type type);
    void (*free)(symbol_variable_t **self);
};

symbol_variable_t *init_symbol_variable(string name, string value, arg_type type, arg_type frame, bool is_function);

#endif // SYMBOL_VARIABLE_H
