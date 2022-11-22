#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE
#include "../scanner/token.h"
typedef struct symbol_table_t symbol_table_t;
typedef struct symbol_node_t symbol_node_t;

/*Symbol tables's code constants*/
enum SYMBOL_TABLE_T
{
    ST_SUCCESS = 0,
    ST_ERR_TYPE,
    ST_ERR
} symbol_table_types;

struct symbol_node_t
{
    /*Variable's name*/
    string name;
    /*Variable's value*/
    string value;
    /*Variable's type*/
    types_t type;
    /*Left node*/
    symbol_node_t *left;
    /*Right node*/
    symbol_node_t *right;
    /**
     * @brief Destructor for the tree to this node
     *
     * @param symbol_node_t pointer to a current symbol_node_t*
     */
    void (*free)(symbol_node_t **);
};

/*Symbol table*/
struct symbol_table_t
{
    /*Name of the scope*/
    string scope_name;
    /*Type of the scope, if it's a function*/
    types_t scope_type;
    /*Symbol table's top scope*/
    symbol_node_t *top;
    /*Symbol table's outer scope*/
    symbol_table_t *next;
    /**
     * @brief Destructor for the tree to this node
     *
     * @param self pointer to a current symbol_table_t
     * @param name variable's name
     * @param value variable's value
     * @param type variable's type
     *
     * @return Symbol table's code
     */
    enum SYMBOL_TABLE_T (*insert)(symbol_table_t *self, string var_name, string var_value, types_t var_type);
    /**
     * @brief Destructor for the symbol table, table will equal NULL
     *
     * @param symbol_table_t pointer to a current symbol_table_t*
     */
    void (*free)(symbol_table_t **self);
    /**
     * @brief Adds scope on top of the stack
     *
     * @param symbol_table_t pointer to a current symbol_table_t*
     */
    void (*push_scope)(symbol_table_t **self,string scope_name, types_t scope_type);
    /**
     * @brief Pops scope from top of the stack, if the stack is empty, table = NULL
     *
     * @param symbol_table_t pointer to a current symbol_table_t*
     */
    void (*pop_scope)(symbol_table_t **self);
    /**
     * @brief Finds variable node by name
     *
     * @param symbol_node_t pointer to a current symbol_table_t
     * @param name variable's name
     *
     * @return symbol table's node from bst with the variable or NULL
     */
    symbol_node_t *(*find)(symbol_table_t *self, string var_name);
};

/**
 * @brief Constructor for symbol table
 *
 * @return newly created symbol table, NOT NULL
 */
symbol_table_t *init_symbol_table();

#endif
