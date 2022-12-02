#include "symbol-table.h"
#include <string.h>

static symbol_variable_t *symbol_table_find(symbol_table_t *self, string name);
static symbol_table_types symbol_table_insert(symbol_table_t *self, string name, string value, arg_type type, bool is_function);
static void free_symbol_node(symbol_node_t **self);
static symbol_node_t *init_symbol_node(string name, string value, arg_type type, arg_type frame, bool is_function);

static string type_to_string(arg_type type)
{
    char *to_string[] = {
        [LF] = "LF",
        [TF] = "TF",
        [GF] = "GF",
        [INT] = "INT",
        [FLOAT] = "FLOAT",
        [STRING] = "STRING",
        [BOOL] = "BOOL",
        [NIL] = "NULL",
    };
    return to_string[type];
}

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
static symbol_table_types
symbol_table_insert(symbol_table_t *self, string name, string value, arg_type type, bool is_function)
{
    symbol_variable_t *var = NULL;
    symbol_node_t *node = self->top;

    if ((var = symbol_table_find(self, name)) != NULL)
    {
        return var->assign(var, value, type);
    }
    if (self->top == NULL)
    {
        self->top = init_symbol_node(name, value, type, self->frame, is_function);
    }
    while (node != NULL)
    {
        if (strcmp(name, node->var->name) < 0)
        {
            if (node->left == NULL)
            {
                node->left = init_symbol_node(name, value, type, self->frame, is_function);
                return ST_INSERT;
            }
            else
            {
                node = node->left;
            }
        }
        else if (strcmp(name, node->var->name) > 0)
        {
            if (node->right == NULL)
            {
                node->right = init_symbol_node(name, value, type, self->frame, is_function);
                return ST_INSERT;
            }
            else
            {
                node = node->right;
            }
        }
        else
        {
            return node->var->assign(node->var, value, type);
        }
    }

    return ST_INSERT;
}

/**
 * @brief Finds variable node by name
 *
 * @param symbol_node_t pointer to a current symbol_table_t
 * @param name variable's name
 *
 * @return symbol table's node from bst with the variable or NULL
 */
static symbol_variable_t *symbol_table_find(symbol_table_t *self, string name)
{
    if (self == NULL)
    {
        return NULL;
    }
    if (self->top == NULL)
    {
        return symbol_table_find(self->next, name);
    }
    symbol_node_t *node = self->top;
    while (node != NULL)
    {
        if (strcmp(name, node->var->name) < 0)
        {
            if (node->left == NULL)
            {
                return symbol_table_find(self->next, name);
            }
            else
            {
                node = node->left;
            }
        }
        else if (strcmp(name, node->var->name) > 0)
        {
            if (node->right == NULL)
            {
                return symbol_table_find(self->next, name);
            }
            else
            {
                node = node->right;
            }
        }
        else
        {
            return node->var;
        }
    }
    return NULL;
}

static arg_type symbol_table_find_g(symbol_table_t *self, string name)
{
    symbol_variable_t *var = symbol_table_find(self, name);

    if (var == NULL)
        return -1;

    return var->frame;
}

/**
 * @brief Destructor for the tree to this node
 *
 * @param symbol_node_t pointer to a current symbol_node_t*
 */
static void free_symbol_node(symbol_node_t **self)
{
    if (*self == NULL)
        return;

    free_symbol_node(&(*self)->left);
    free_symbol_node(&(*self)->right);

    (*self)->var->free(&(*self)->var);
    free(self);

    self = NULL;
}

/**
 * @brief Adds scope on top of the stack
 *
 * @param symbol_table_t pointer to a current symbol_table_t*
 */
static void push_frame(symbol_table_t **self, string name, arg_type type)
{
    symbol_table_t *new_scope = init_symbol_table(TF);
    malloc_s(new_scope->frame_name, char, strlen(name));
    strcpy(new_scope->frame_name, name);
    new_scope->top = NULL;
    new_scope->frame_type = type;
    new_scope->next = *self;
    *self = new_scope;
}

/**
 * @brief Pops scope from top of the stack, if the stack is empty, table = NULL
 *
 * @param symbol_table_t pointer to a current symbol_table_t*
 */
static void pop_frame(symbol_table_t **self)
{
    if (*self != NULL)
    {
        symbol_table_t *delete_scope = *self;
        *self = (*self)->next;
        if (delete_scope->top != NULL)
        {
            delete_scope->top->free(&delete_scope->top);
        }
        free(delete_scope->frame_name);
        free(delete_scope);
    }
}

static void symbol_table_traverse(symbol_node_t *root)
{
    if (root != NULL)
    {
        printf("   %s|%s: %s = %s ", type_to_string(root->var->frame), root->var->name, type_to_string(root->var->type), root->var->value);
        if (root->var->is_function)
        {
            printf("( ");
            for (symbol_variable_t *arg = root->var->arg_next; arg != NULL; arg = arg->arg_next)
                printf("%s|%s: %s = %s, ", type_to_string(arg->frame), arg->name, type_to_string(arg->type), arg->value);
            printf(")");
        }
        printf("\n");
        symbol_table_traverse(root->left);
        symbol_table_traverse(root->right);
    }
}

void symbol_table_debug(symbol_table_t *self)
{
    if (self == NULL)
    {
        return;
    }
    printf("SCOPE %s, %s : %s\n", type_to_string(self->frame), self->frame_name, type_to_string(self->frame_type));
    symbol_table_traverse(self->top);
    symbol_table_debug(self->next);
}

/**
 * @brief Destructor for the symbol table, table will equal NULL
 *
 * @param symbol_table_t pointer to a current symbol_table_t*
 */
static void free_symbol_table(symbol_table_t **self)
{

    while (*self != NULL)
    {
        (*self)->pop_frame(self);
    }
}

static symbol_node_t *init_symbol_node(string name, string value, arg_type type, arg_type frame, bool is_function)
{
    symbol_node_t *node = NULL;
    malloc_s(node, symbol_node_t, 1);
    node->left = NULL;
    node->right = NULL;
    node->var = init_symbol_variable(name, value, type, frame, is_function);
    node->free = free_symbol_node;
    return node;
}

symbol_table_t *init_symbol_table(arg_type frame)
{
    symbol_table_t *table = NULL;
    malloc_s(table, symbol_table_t, 1);
    table->frame_type = NIL;
    table->frame = frame;
    table->top = NULL;
    table->next = NULL;
    table->frame_name = NULL;
    table->find_g = symbol_table_find_g;
    table->debug = symbol_table_debug;
    table->insert = symbol_table_insert;
    table->free = free_symbol_table;
    table->push_frame = push_frame;
    table->pop_frame = pop_frame;
    table->find = symbol_table_find;
    return table;
}
