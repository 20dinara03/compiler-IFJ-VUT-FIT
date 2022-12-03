#include "expr_parser.h"
// stack

#define FREE_STACK(_return)     \
    do                          \
    {                           \
        expr_stack_free(stack); \
        return _return;         \
    } while (0)

int prec_table[8][8] =
    {
        {R, L, R, L, R, L, N, R},
        {R, R, R, L, R, L, N, R},
        {L, L, N, L, R, L, L, R},
        {L, L, L, L, E, L, L, N},
        {R, R, R, N, R, N, R, R},
        {R, R, R, N, R, N, R, R},
        {N, N, R, L, R, L, R, R},
        {L, L, L, L, N, L, L, N}};

//   //|+- | */| r | ( | ) | i |   $ |
// 	{ R , S , S , S , R , S ,   R }, /// +-
// 	{ R , R , R , S , R , S ,   R }, /// */
// 	{ S , S , S , S , R , S ,   R }, /// r (realtion operators) = <> <= < >= >
// 	{ S , S , S , S , E , S ,   N }, /// (
// 	{ R , R , R , N , R , N ,   R }, /// )
// 	{ R , R , R , N , R , N ,   R }, /// i (id, int, double, string)

// 	{ S , S , S , S , N , S ,   N }  /// $

void expr_stack_init(expr_stack_t *stack)
{
    stack->top = NULL;
}

bool expr_stack_push(expr_stack_t *stack, stack_symbols symbol, non_term_type_t non_term_type, token_t *token)
{
    expr_stack_item_t *new_item = NULL;

    malloc_s(new_item, expr_stack_item_t, 1);

    new_item->symbol = symbol;
    new_item->non_term_type = non_term_type;
    new_item->token = token;
    new_item->next = stack->top;
    stack->top = new_item;

    return true;
}

bool expr_stack_pop(expr_stack_t *stack)
{
    if (stack->top != NULL)
    {
        expr_stack_item_t *tmp = stack->top;
        stack->top = tmp->next;
        free(tmp);

        return true;
    }
    return false;
}

void expr_stack_pop_some(expr_stack_t *stack, int amount)
{
    for (int i = 0; i < amount; i++)
    {
        expr_stack_pop(stack);
    }
}

expr_stack_item_t *expr_stack_top(expr_stack_t *stack)
{
    return stack->top;
}

expr_stack_item_t *expr_stack_top_term(expr_stack_t *stack)
{
    for (expr_stack_item_t *tmp = stack->top; tmp != NULL; tmp = tmp->next)
    {
        if (tmp->symbol < STOP)
            return tmp;
    }

    return NULL;
}

bool expr_stack_insert_after_term(expr_stack_t *stack, stack_symbols symbol, non_term_type_t non_term_type, token_t *token)
{
    expr_stack_item_t *prev = NULL;

    for (expr_stack_item_t *tmp = stack->top; tmp != NULL; tmp = tmp->next)
    {
        if (tmp->symbol < STOP)
        {
            expr_stack_item_t *new_item = (expr_stack_item_t *)malloc(sizeof(expr_stack_item_t));

            if (new_item == NULL)
                return false;

            new_item->symbol = symbol;
            new_item->non_term_type = non_term_type;
            new_item->token = token;

            if (prev == NULL)
            {
                new_item->next = stack->top;
                stack->top = new_item;
            }
            else
            {
                new_item->next = prev->next;
                prev->next = new_item;
            }

            return true;
        }

        prev = tmp;
    }

    return false;
}

void expr_stack_free(expr_stack_t *stack)
{
    while (expr_stack_pop(stack));
}
// stack

stack_symbols get_symbol_from_token(token_t *token)
{
    switch (token->type)
    {
    case OPERATOR_PLUS:
        return PLUS;
    case OPERATOR_MINUS:
        return MINUS;
    case OPERATOR_MULTIPLY:
        return MUL;
    case OPERATOR_DIVIDE:
        return DIV;
    case OPERATOR_EQUAL:
        return EQ;
    case OPERATOR_LESS:
        return LESS;
    case OPERATOR_GREATER:
        return MORE;
    case OPERATOR_LESS_OR_EQUAL:
        return L_EQ;
    case OPERATOR_GREATER_OR_EQUAL:
        return M_EQ;
    case OPERATOR_NOT_EQUAL:
        return N_EQ;
    case OPERATOR_LEFT_BRACKET:
        return LEFT_BRACKET;
    case OPERATOR_RIGHT_BRACKET:
        return RIGHT_BRACKET;
    case VARIABLE_IDENTIFIER:
        return ID;
    case INT_LITERAL:
        return INT_NUMBER;
    case DOUBLE_LITERAL:
        return DOUBLE_NUMBER;
    case STRING_LITERAL:
        return STRING_LINE;
    case KEYWORD_NULL:
        return NULL_TYPE;
    case OPERATOR_POINT:
        return CONC;
    // case IDENTIFIER:
    //     if parseFunctionCall()
    default:
        return DOLLAR;
    }
}

prec_table_index get_prec_table_index(stack_symbols symbol)
{
    switch (symbol)
    {
    case PLUS:
    case MINUS:
        return PREC_PLUS_MINUS;
    case MUL:
    case DIV:
        return PREC_MUL_DIV;
    case EQ:
    case N_EQ:
    case L_EQ:
    case LESS:
    case M_EQ:
    case MORE:
        return PREC_REL;
    case LEFT_BRACKET:
        return PREC_LEFT_BR;
    case RIGHT_BRACKET:
        return PREC_RIGHT_BR;
    case ID:
    case INT_NUMBER:
    case DOUBLE_NUMBER:
    case STRING_LINE:
    case NULL_TYPE:
        return PREC_ID;
    case CONC:
        return PREC_CONC;
    default:
        return PREC_DOLLAR;
    }
}

int num_of_symbols_after_stop(bool *stop_found, expr_stack_t *stack)
{
    expr_stack_item_t *tmp = expr_stack_top(stack);
    int count = 0;

    while (tmp != NULL)
    {
        if (tmp->symbol != STOP)
        {
            *stop_found = false;
            count++;
        }
        else
        {
            *stop_found = true;
            break;
        }

        tmp = tmp->next;
    }

    return count;
}

bool test_rule(int num, expr_stack_item_t *op1, expr_stack_item_t *op2, expr_stack_item_t *op3)
{
    switch (num)
    {
    case 1:
        // rule E -> i
        if (op1->symbol == ID || op1->symbol == INT_NUMBER || op1->symbol == DOUBLE_NUMBER || op1->symbol == STRING_LINE)
            return true;

        return false;

    case 3:
        // rule E -> (E)
        if (op1->symbol == LEFT_BRACKET && op2->symbol == E_NONTERM && op3->symbol == RIGHT_BRACKET)
            return true;

        if (op1->symbol == E_NONTERM && op3->symbol == E_NONTERM)
        {
            switch (op2->symbol)
            {
            // rule E -> E + E
            case PLUS:
            // rule E -> E - E
            case MINUS:
            // rule E -> E * E
            case MUL:
            // rule E -> E / E
            case DIV:
            // rule E -> E . E
            case CONC:
            // rule E -> E = E
            case EQ:
            // rule E -> E <> E
            case N_EQ:
            // rule E -> E <= E
            case L_EQ:
            // rule E -> E < E
            case LESS:
            // rule E -> E >= E
            case M_EQ:
            // rule E -> E > E
            case MORE:
                return true;
            // invalid operator
            default:
                return false;
            }
        }
        return false;
    }
    return false;
}

bool reduce_by_rule(expr_stack_t *stack, parser_t* parser)
{
    expr_stack_item_t *op1 = NULL;
    expr_stack_item_t *op2 = NULL;
    expr_stack_item_t *op3 = NULL;
    bool found = false;
    bool is_rule = false;
    bool comp_types = false;
    non_term_type_t non_term_type = NOT_E;

    int count = num_of_symbols_after_stop(&found, stack);

    if (count == 1 && found)
    {
        op1 = stack->top;
        is_rule = test_rule(count, op1, NULL, NULL);
        comp_types = semantic_analysis(parser, op1, NULL, NULL, &non_term_type);
    }
    else if (count == 3 && found)
    {
        op1 = stack->top->next->next;
        op2 = stack->top->next;
        op3 = stack->top;
        is_rule = test_rule(count, op1, op2, op3);
        comp_types = semantic_analysis(parser, op1, NULL, NULL, &non_term_type);
    }
    else
        return false;

    if (is_rule && comp_types)
    {
        expr_stack_pop_some(stack, count + 1);
        expr_stack_push(stack, E_NONTERM, non_term_type, non_term_type);
    }
    else
    {
        return false;
    }

    return true;
}

void print_stack(expr_stack_t* stack){
    for(expr_stack_item_t*item = stack->top; item != NULL; item = item->next){
        printf("%d, ",item->symbol);
    }
    printf("\n");
}


bool semantic_analysis(parser_t* parser, expr_stack_item_t *op1, expr_stack_item_t *op2, expr_stack_item_t *op3, non_term_type_t* non_term_type)
{
	bool op1_to_double = false;
	bool op3_to_double = false;
	bool op1_to_integer = false;
	bool op3_to_integer = false;

    if (op2 == NULL && op3 == NULL){
        symbol_variable_t* symbol_variable;
        switch (op1->symbol){
            case INT_NUMBER:
                non_term_type = INT_E;
            case DOUBLE_NUMBER:
                non_term_type = DOUBLE_E;
            case STRING_LINE:
                non_term_type = STRING_E;
            case KEYWORD_NULL:
                non_term_type = NULL_E;
            case ID:
                symbol_variable = parser->symbol_table->find(parser->symbol_table, op1->token->text);
                switch(symbol_variable->type){
                    case INT:
                        non_term_type = INT_E;
                    case STRING:
                        non_term_type = STRING_E;
                    case FLOAT:
                        non_term_type = DOUBLE_E;
                    case NIL:
                        non_term_type = NULL_E;
                }
            default:
                return false;
        }
    }
    else if(op1->symbol == E_NONTERM && op3->symbol == E_NONTERM){
        switch(op2->symbol){
            case PLUS:
            case MINUS:
            //switch type of non_term_type, check op1 op3 return if false if not return then generace
        }
    }
	// if (rule == OPERAND)
	// {
	// 	if (op1->data_type == TYPE_UNDEFINED)
	// 		return SEM_ERR_UNDEFINED_VAR;

	// 	if (op1->data_type == TYPE_BOOL)
	// 		return SEM_ERR_TYPE_COMPAT;
	// }

	// if (rule == LBR_NT_RBR)
	// {
	// 	if (op2->data_type == TYPE_UNDEFINED)
	// 		return SEM_ERR_UNDEFINED_VAR;
	// }

	// if (rule != OPERAND && rule != LBR_NT_RBR)
	// {
	// 	if (op1->data_type == TYPE_UNDEFINED || op3->data_type == TYPE_UNDEFINED)
	// 		return SEM_ERR_UNDEFINED_VAR;

	// 	if (op1->data_type == TYPE_BOOL || op3->data_type == TYPE_BOOL)
	// 		return SEM_ERR_TYPE_COMPAT;
	// }

	// switch (rule)
	// {
	// case OPERAND:
	// 	*final_type = op1->data_type;
	// 	break;

	// case LBR_NT_RBR:
	// 	*final_type = op2->data_type;
	// 	break;

	// case NT_PLUS_NT:
	// case NT_MINUS_NT:
	// case NT_MUL_NT:
	// 	if (op1->data_type == TYPE_STRING && op3->data_type == TYPE_STRING && rule == NT_PLUS_NT)
	// 	{
	// 		*final_type = TYPE_STRING;
	// 		break;
	// 	}

	// 	if (op1->data_type == TYPE_INT && op3->data_type == TYPE_INT)
	// 	{
	// 		*final_type = TYPE_INT;
	// 		break;
	// 	}

	// 	if (op1->data_type == TYPE_STRING || op3->data_type == TYPE_STRING)
	// 		return SEM_ERR_TYPE_COMPAT;

	// 	*final_type = TYPE_DOUBLE;

	// 	if (op1->data_type == TYPE_INT)
	// 		retype_op1_to_double = true;

	// 	if (op3->data_type == TYPE_INT)
	// 		retype_op3_to_double = true;

	// 	break;

	// case NT_DIV_NT:
	// 	*final_type = TYPE_DOUBLE;

	// 	if (op1->data_type == TYPE_STRING || op3->data_type == TYPE_STRING)
	// 		return SEM_ERR_TYPE_COMPAT;

	// 	if (op1->data_type == TYPE_INT)
	// 		retype_op1_to_double = true;

	// 	if (op3->data_type == TYPE_INT)
	// 		retype_op3_to_double = true;

	// 	break;

	// case NT_IDIV_NT:
	// 	*final_type = TYPE_INT;

	// 	if (op1->data_type == TYPE_STRING || op3->data_type == TYPE_STRING)
	// 		return SEM_ERR_TYPE_COMPAT;

	// 	if (op1->data_type == TYPE_DOUBLE)
	// 		retype_op1_to_integer = true;

	// 	if (op3->data_type == TYPE_DOUBLE)
	// 		retype_op3_to_integer = true;

	// 	break;

	// case NT_EQ_NT:
	// case NT_NEQ_NT:
	// case NT_LEQ_NT:
	// case NT_LTN_NT:
	// case NT_MEQ_NT:
	// case NT_MTN_NT:
	// 	*final_type = TYPE_BOOL;

	// 	if (op1->data_type == TYPE_INT && op3->data_type == TYPE_DOUBLE)
	// 		retype_op1_to_double = true;

	// 	else if (op1->data_type == TYPE_DOUBLE && op3->data_type == TYPE_INT)
	// 		retype_op3_to_double = true;

	// 	else if (op1->data_type != op3->data_type)
	// 		return SEM_ERR_TYPE_COMPAT;

	// 	break;

	// default:
	// 	break;
	// }

	// if (retype_op1_to_double)
	// {
	// 	GENERATE_CODE(generate_stack_op2_to_double);
	// }

	// if (retype_op3_to_double)
	// {
	// 	GENERATE_CODE(generate_stack_op1_to_double);
	// }

	// if (retype_op1_to_integer)
	// {
	// 	GENERATE_CODE(generate_stack_op2_to_integer);
	// }

	// if (retype_op3_to_integer)
	// {
	// 	GENERATE_CODE(generate_stack_op1_to_integer);
	// }

	// return SYNTAX_OK;
}





bool expression(parser_t *parser)
{
    int count_brackets = 0;
    expr_stack_t *stack = (expr_stack_t *)malloc(sizeof(expr_stack_t));
    expr_stack_init(stack);
    if (!expr_stack_push(stack, DOLLAR, NOT_E, NULL))
        FREE_STACK(false);

    expr_stack_item_t *top_stack_terminal;
    stack_symbols actual_symbol;

    bool success = false;
    do
    {
        if ((parser->scanner->current_token->type == IDENTIFIER)){
            if (!parseFunctionCall(parser)){
                //printf("%d",parser->scanner->current_token->type);
                //printf("false1\n");
                FREE_STACK(false);
            }
            else{
                //printf("%d",parser->scanner->current_token->type);
                //printf("true1\n");
                expr_stack_free(stack);
                return true;
            }
        }

        //printf("bravck %d\n", count_brackets);
        actual_symbol = get_symbol_from_token(parser->scanner->current_token);
        top_stack_terminal = expr_stack_top_term(stack);

        if (top_stack_terminal == NULL){
            //printf("%d",parser->scanner->current_token->type);
            //printf("false1\n");
            FREE_STACK(false);
        }
        //print_stack(stack);
        //printf("Vstup %d\n",actual_symbol);
        // //printf("brscket %d\n",count_brackets);
        switch (prec_table[get_prec_table_index(top_stack_terminal->symbol)][get_prec_table_index(actual_symbol)])
        {
        case L:
            if (!expr_stack_insert_after_term(stack, STOP, NOT_E, NULL)){
                //printf("%d",parser->scanner->current_token->type);
                //printf("false3\n");
                FREE_STACK(false);
            }

            if (!expr_stack_push(stack, actual_symbol, NOT_E, parser->scanner->current_token)){
                //printf("%d",parser->scanner->current_token->type);
                //printf("false4\n");
                FREE_STACK(false);
            }

            if ((!parser->scanner->get_next_token(parser->scanner))){
                //printf("%d",parser->scanner->current_token->type);
                //printf("false5\n");
                FREE_STACK(false);
            }
            break;

        case E:
            expr_stack_push(stack, actual_symbol, NOT_E, parser->scanner->current_token);

            if ((!parser->scanner->get_next_token(parser->scanner))){
                //printf("%d",parser->scanner->current_token->type);
                //printf("false6\n");
                FREE_STACK(false);
            }
            break;

        case R:
            if ((!reduce_by_rule(parser, stack))){
                //printf("%d",parser->scanner->current_token->type);
                //printf("false7\n");
                FREE_STACK(false);
            }
            break;

        case N:
            if (actual_symbol == DOLLAR && top_stack_terminal->symbol == DOLLAR)
                success = true;
            else{
                //printf("%d",parser->scanner->current_token->type);
                //printf("false8\n");
                FREE_STACK(false);
            }
            break;
        }
        if ((expr_stack_top(stack)->symbol != E_NONTERM) && (parser->scanner->current_token->type == OPERATOR_LEFT_BRACKET)){
            count_brackets += 1;
        }
        if ((expr_stack_top(stack)->symbol != E_NONTERM) && (parser->scanner->current_token->type == OPERATOR_RIGHT_BRACKET)){
            count_brackets -= 1;
            if (count_brackets < 0){
                // //printf("%d",parser->scanner->current_token->type);
                // //printf("false2\n");
                expr_stack_free(stack);
                return true;
            }
        }
    } while (!success);

    // //printf("%d", expr_stack_top(stack)->symbol);

    expr_stack_item_t *final_non_terminal = expr_stack_top(stack);
    if (final_non_terminal == NULL){
        //printf("%d",parser->scanner->current_token->type);
        //printf("false9\n");
        FREE_STACK(false);
    }
    if (final_non_terminal->symbol != E_NONTERM){
        //printf("%d",parser->scanner->current_token->type);
        //printf("false10\n");
        FREE_STACK(false);
    }
    //printf("%d",parser->scanner->current_token->type);
    //printf("true10\n");

    
    expr_stack_free(stack);
    return true;
}
