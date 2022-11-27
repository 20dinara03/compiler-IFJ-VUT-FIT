#include "symbol-table.h"
#include <string.h>

symbol_node_t *init_symbol_node(string name, string value, arg_type type);

enum SYMBOL_TABLE_T symbol_table_insert(symbol_table_t *self, string name, string value, arg_type type)
{
    symbol_node_t *node = self->top;
    if (self->top == NULL)
    {
        self->top = init_symbol_node(name, value, type);
    }
    else
    {
        while (node != NULL)
        {
            if (strcmp(name, node->name) < 0)
            {
                if (node->left == NULL)
                {
                    node->left = init_symbol_node(name, value, type);
                    break;
                }
                else
                {
                    node = node->left;
                }
            }
            else if (strcmp(name, node->name) > 0)
            {
                if (node->right == NULL)
                {
                    node->right = init_symbol_node(name, value, type);
                    break;
                }
                else
                {
                    node = node->right;
                }
            }
            else
            {
                if (node->type != type)
                {
                    return ST_ERR_TYPE; // Type error
                }
                strcpy(node->value, value);
                break;
            }
        }
    }
    return ST_SUCCESS;
}

symbol_node_t *symbol_table_find(symbol_table_t *self, string name)
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
        if (strcmp(name, node->name) < 0)
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
        else if (strcmp(name, node->name) > 0)
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
            return node;
        }
    }
    return NULL;
}

void free_symbol_node(symbol_node_t **self)
{
    if (*self == NULL)
        return;

    free_symbol_node(&(*self)->left);
    free_symbol_node(&(*self)->right);

    free((*self)->name);
    free((*self)->value);
    free(*self);

    *self = NULL;
}

void free_symbol_table(symbol_table_t **self)
{
    while (*self != NULL)
    {
        (*self)->pop_scope(self);
    }
}

symbol_node_t *init_symbol_node(string name, string value, arg_type type)
{
    symbol_node_t *node = NULL;
    memo_allocate(node, symbol_node_t, 1);
    memo_allocate(node->name, char, strlen(name));
    memo_allocate(node->value, char, strlen(value));
    strcpy(node->name, name);
    strcpy(node->value, value);
    node->type = type;
    node->left = node->right = NULL;
    node->free = free_symbol_node;
    return node;
}

void push_scope(symbol_table_t **self, string name, arg_type type)
{
    symbol_table_t *new_scope = init_symbol_table();
    memo_allocate(new_scope->scope_name, char, strlen(name));
    strcpy(new_scope->scope_name, name);
    new_scope->top = NULL;
    new_scope->scope_type = type;
    new_scope->next = *self;
    *self = new_scope;
}

void pop_scope(symbol_table_t **self)
{
    if (*self != NULL)
    {
        symbol_table_t *delete_scope = *self;
        *self = (*self)->next;
        if (delete_scope->top != NULL)
        {
            delete_scope->top->free(&delete_scope->top);
        }
        free(delete_scope->scope_name);
        free(delete_scope);
    }
}

symbol_table_t *init_symbol_table()
{
    symbol_table_t *table = NULL;
    memo_allocate(table, symbol_table_t, 1);
    table->scope_type = NONE;
    table->scope_name = NULL;
    table->top = NULL;
    table->next = NULL;
    table->insert = symbol_table_insert;
    table->free = free_symbol_table;
    table->push_scope = push_scope;
    table->pop_scope = pop_scope;
    table->find = symbol_table_find;
    return table;
}
