#ifndef EXPR_PARSER_H
#define EXPR_PARSER_H

#include "parser.h"
#include <stdio.h>



typedef struct expr_stack_item_t expr_stack_item_t;

typedef enum
{
    L, // <
    E, // =
    R, // >
    N, // NONE
} prec_table_actions;

typedef enum
{
    PREC_PLUS_MINUS, // 0 +-
    PREC_MUL_DIV,    // 1 */
    PREC_REL,        // 2 r
    PREC_LEFT_BR,    // 3 (
    PREC_RIGHT_BR,   // 4 )
    PREC_ID,         // 5 id
    PREC_CONC,       // 6 .
    PREC_DOLLAR,     // 7 $
} prec_table_index;

typedef enum
{
	PLUS,			// +
	MINUS,			// -
	MUL,			// *
	DIV,			// /
	EQ,				// ===
    CONC,           // .
	N_EQ,			// !==
	L_EQ,			// <=
	LESS,			// <
	M_EQ,			// >=
	MORE,			// >
	LEFT_BRACKET,	// (
	RIGHT_BRACKET,	// )
	ID,		// ID
	INT_NUMBER,		// int
	DOUBLE_NUMBER,	// double
	STRING_LINE,			// string
    NULL_TYPE,
	DOLLAR,			// $
	STOP,			// <
	E_NONTERM		// non-terminal
} stack_symbols;

typedef enum
{
	INT_E,		// int
	DOUBLE_E,	// double
	STRING_E,			// string
    NULL_E,
	BOOL_E,
	NOT_E		// non-terminal
} non_term_type_t;

typedef struct expr_stack_item_t
{
	string actual_token_text;
    non_term_type_t non_term_type; 
    stack_symbols symbol;
    expr_stack_item_t *next;
} expr_stack_item_t;

typedef struct
{
    expr_stack_item_t *top;
} expr_stack_t;

typedef struct
{
    parser_t *parser;
    expr_stack_t *stack;
} expr_parser_t;

bool expression(parser_t *parser, expr_type_t expr_type, string variable_name);
bool semantic_analysis(parser_t* parser, expr_stack_item_t *op1, expr_stack_item_t *op2, expr_stack_item_t *op3, non_term_type_t* non_term_type);

#define arg(type, name) new_arg(type, name)
#define simple_arg(name) new_simple_arg(name)
#define label(type) new_label(self->code_stack, type, -1)
#define simple_label(name) new_simple_label(name)

#define frame_add_line self->get_current_block(self)->add_line
#define as self->get_current_block(self), self->get_current_stack(self)->templater->
#define new_code_frame                                                                          \
    self->get_current_stack(self)->push(self->get_current_stack(self), self->definition_stage); \
    bool pass;

#define end_code_frame \
    self->get_current_stack(self)->pop(self->get_current_stack(self), self->definition_stage);

#endif
