#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE
#include "./symbol-variable.h"
typedef struct symbol_table_t symbol_table_t;
typedef struct symbol_node_t symbol_node_t;

struct symbol_node_t
{
    symbol_variable_t *var;
    /*Left node*/
    symbol_node_t *left;
    /*Right node*/
    symbol_node_t *right;
    void (*free)(symbol_node_t **);
};

/*Symbol table*/
struct symbol_table_t
{
    /*Name of the scope*/
    string frame_name;
    /*Type of the scope, if it's a function*/
    arg_type frame_type;
    /*Symbol table's top scope*/
    symbol_node_t *top;
    /*Symbol table's outer scope*/
    symbol_table_t *next;

    arg_type frame;

    void (*debug)(symbol_table_t *self);
    symbol_table_types (*insert)(symbol_table_t *self, string var_name, string var_value, arg_type var_type, bool is_function);
    symbol_variable_t *(*find)(symbol_table_t *self, string var_name);
    arg_type (*find_g)(symbol_table_t *self, string name);
    void (*push_frame)(symbol_table_t **self, string frame_name, arg_type frame_type);
    void (*pop_frame)(symbol_table_t **self);
    void (*free)(symbol_table_t **self);
};

/**
 * @brief Constructor for symbol table
 *
 * @return newly created symbol table, NOT NULL
 */
symbol_table_t *init_symbol_table(arg_type frame);

#endif
