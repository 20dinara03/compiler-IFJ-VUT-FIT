#include "expr_parser.h"
#define st self->symbol_table
#define st_get(name) self->symbol_table->find(self->symbol_table, name)
#define st_get_type(name) self->symbol_table->find_g(self->symbol_table, name)
#define st_assign(target, name, type) self->symbol_table->find(self->symbol_table, name)->assign(self->symbol_table->find(self->symbol_table, name), nane, type)
#define st_debug self->symbol_table->debug(self->symbol_table)
#define current_token self->scanner->current_token
// stack

#define FREE_STACK(_return)     \
    do                          \
    {                           \
        expr_stack_free(stack); \
        return _return;         \
    } while (0)

// #define scope_var(name) self->symbol_table->insert(self->symbol_table, name, "0", NIL, false)

void print_stack(expr_stack_t *stack)
{
    const char *dict_types[] = {
        [PLUS] = "PLUS",                   // +
        [MINUS] = "MINUS",                 // -
        [MUL] = "MUL",                     // *
        [DIV] = "DIV",                     // /
        [EQ] = "EQ",                       // ===
        [CONC] = "CONC",                   // .
        [N_EQ] = "N_EQ",                   // !==
        [L_EQ] = "L_EQ",                   // <=
        [LESS] = "LESS",                   // <
        [M_EQ] = "M_EQ",                   // >=
        [MORE] = "MORE",                   // >
        [LEFT_BRACKET] = "LEFT_BRACKET",   // (
        [RIGHT_BRACKET] = "RIGHT_BRACKET", // )
        [ID] = "ID",                       // ID
        [INT_NUMBER] = "INT_NUMBER",       // int
        [DOUBLE_NUMBER] = "DOUBLE_NUMBER", // double
        [STRING_LINE] = "STRING_LINE",     // string
        [NULL_TYPE] = "NULL_TYPE",
        [DOLLAR] = "DOLLAR",      // $
        [STOP] = "STOP",          // <
        [E_NONTERM] = "E_NONTERM" // non-terminal
    };
    printf("STACK_LOG:\t");
    for (expr_stack_item_t *item = stack->top; item != NULL; item = item->next)
    {
        printf("%s,\t", dict_types[item->symbol]);
    }
    printf("\n");
}

int prec_table[8][8] =
    {
        //|+- | */| r | ( | ) | i |   $ |
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

arg_type retype_E(non_term_type_t non_term_type)
{
    switch (non_term_type)
    {
    case INT_E:
        return INT;
    case DOUBLE_E:
        return FLOAT;
    case STRING_E:
        return STRING;
    case NULL_E:
        return NIL;
    default:
        exit_failure(-1);
        return -1;
    }
}

void expr_stack_init(expr_stack_t *stack)
{
    stack->top = NULL;
}

bool expr_stack_push(expr_stack_t *stack, stack_symbols symbol, non_term_type_t non_term_type, string actual_token_text)
{
    expr_stack_item_t *new_item = NULL;

    malloc_s(new_item, expr_stack_item_t, 1);

    new_item->symbol = symbol;
    new_item->non_term_type = non_term_type;
    new_item->actual_token_text = actual_token_text;
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

bool expr_stack_insert_after_term(expr_stack_t *stack, stack_symbols symbol, non_term_type_t non_term_type, string actual_token_text)
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
            new_item->actual_token_text = actual_token_text;

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
    while (expr_stack_pop(stack))
        ;
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

bool reduce_by_rule(parser_t *parser, expr_stack_t *stack)
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
        comp_types = semantic_analysis(parser, op1, op2, op3, &non_term_type);
    }
    else
    {
        return false;
    }
    // printf("comp_type %d", comp_types);
    if (is_rule && comp_types)
    {
        expr_stack_pop_some(stack, count + 1);
        expr_stack_push(stack, E_NONTERM, non_term_type, NULL);
    }
    else
    {
        return false;
    }

    return true;
}

bool semantic_analysis(parser_t *self, expr_stack_item_t *op1, expr_stack_item_t *op2, expr_stack_item_t *op3, non_term_type_t *non_term_type)
{
    // printf("%s", op1->actual_token_text);
    bool op1_to_double = false;
    bool op3_to_double = false;
    // bool op1_to_int = false;
    // bool op3_to_int = false;
    op1_to_double = op1_to_double;
    op3_to_double = op3_to_double;
    // printf("insemantic\n");
    if (op2 == NULL && op3 == NULL)
    {
        symbol_variable_t *symbol_variable = NULL;
        switch (op1->symbol)
        {
        case INT_NUMBER:
            *non_term_type = INT_E;
            break;
        case DOUBLE_NUMBER:
            *non_term_type = DOUBLE_E;
            break;
        case STRING_LINE:
            *non_term_type = STRING_E;
            break;
        case NULL_TYPE:
            *non_term_type = NULL_E;
            break;
        case ID:
            // printf("inid\n");
            // printf("%p\n", parser->symbol_table);
            // st_debug;
            // printf("%s\n",op1->actual_token_text);
            symbol_variable = st_get(op1->actual_token_text);

            if (symbol_variable == NULL)
            {
                // FREE STACK
                exit_failure(SEMANTIC_UNDEFINED_VAR_ERR);
            }
            switch (symbol_variable->type)
            {
            case INT:
                *non_term_type = INT_E;
                break;
            case STRING:
                *non_term_type = STRING_E;
                break;
            case FLOAT:
                *non_term_type = DOUBLE_E;
                break;
            case NIL:
                *non_term_type = NULL_E;
                break;
            default:
                // printf("1exit\n");
                return false;
            }
            break;
        default:
            // printf("2exit\n");
            return false;
        }
        frame_add_line(as PUSHS(new_arg(retype_E(*non_term_type), op1->actual_token_text)));
    }
    else if (op1->symbol == E_NONTERM && op3->symbol == E_NONTERM)
    {
        switch (op2->symbol)
        {
        case PLUS:
        case MINUS:
        case MUL:
        case DIV:
            
            if ((op1->non_term_type >= 2) || (op3->non_term_type >= 2))
            {
                return false;
            }
            if (op1->non_term_type == INT_E && op3->symbol == DIV && op3->non_term_type == INT_E)
            {
                op3_to_double = true;
                op1_to_double = true;
                *non_term_type = DOUBLE_E;
            }
            if (op1->non_term_type != op3->non_term_type)
            {
                *non_term_type = DOUBLE_E;
                if (op1->non_term_type == DOUBLE_E)
                {
                    frame_add_line(as INT2FLOATS());
                    op3_to_double = true;
                }
                else
                {
                    // frame_add_line(as INT2FLOATS());
                    op1_to_double = true;
                }
            }
            if(op2->symbol == PLUS){
                frame_add_line(as ADDS());
            }
            break;
        default:
            break;

            // case CONC:
            //     //if 1.1
            // case EQ:

            // case N_EQ:

            // case L_EQ:

            // case LESS:

            // case M_EQ:

            // case MORE:
            // switch type of non_term_type, check op1 op3 return if false if not return then generace
        }
    }
    return true;
}

bool expression(parser_t *self, expr_type_t expr_type, string variable_name)
{
    int count_brackets = 0;
    expr_stack_t *stack = (expr_stack_t *)malloc(sizeof(expr_stack_t));
    expr_stack_init(stack);
    expr_stack_push(stack, DOLLAR, NOT_E, NULL);

    expr_stack_item_t *top_stack_terminal;
    stack_symbols actual_symbol;

    bool success = false;
    do
    {
        string actual_token_text = NULL;
        if ((current_token->type == IDENTIFIER))
        {
            malloc_s(actual_token_text, char, strlen(current_token->text));
            strcpy(actual_token_text, current_token->text);
            if (!parseFunctionCall(self))
            {
                // printf("%d",parser->scanner->current_token->type);
                // printf("false1\n");
                FREE_STACK(false);
            }
            else
            {
                // printf("%d",parser->scanner->current_token->type);
                // printf("true1\n");
                actual_symbol = ID;
            }
        }
        else
        {
            actual_symbol = get_symbol_from_token(current_token);
            malloc_s(actual_token_text, char, strlen(current_token->text));
            strcpy(actual_token_text, current_token->text);
        }

        // printf("bravck %d\n", count_brackets);

        top_stack_terminal = expr_stack_top_term(stack);

        if (top_stack_terminal == NULL)
        {
            // printf("%d",parser->scanner->current_token->type);
            // printf("false1\n");
            FREE_STACK(false);
        }
        // print_stack(stack);
        // printf("Vstup %d\n",actual_symbol);
        //  //printf("brscket %d\n",count_brackets);
        switch (prec_table[get_prec_table_index(top_stack_terminal->symbol)][get_prec_table_index(actual_symbol)])
        {
        case L:
            // printf("ljkgyfkutckucfcty");

            if (!expr_stack_insert_after_term(stack, STOP, NOT_E, NULL))
            {
                // printf("%d",parser->scanner->current_token->type);
                // printf("false3\n");
                FREE_STACK(false);
            }

            if (!expr_stack_push(stack, actual_symbol, NOT_E, actual_token_text))
            {
                // printf("%d",parser->scanner->current_token->type);
                // printf("false4\n");
                FREE_STACK(false);
            }

            if ((!self->scanner->get_next_token(self->scanner)))
            {
                // printf("%d",parser->scanner->current_token->type);
                // printf("false5\n");
                FREE_STACK(false);
            }
            // print_stack(stack);
            // printf("zahodyt");
            break;

        case E:
            expr_stack_push(stack, actual_symbol, NOT_E, actual_token_text);

            if ((!self->scanner->get_next_token(self->scanner)))
            {
                // printf("%d",parser->scanner->current_token->type);
                // printf("false6\n");
                FREE_STACK(false);
            }
            break;

        case R:
            if ((!reduce_by_rule(self, stack)))
            {
                // printf("%d",parser->scanner->current_token->type);
                // printf("false7\n");
                FREE_STACK(false);
            }
            break;

        case N:
            if (actual_symbol == DOLLAR && top_stack_terminal->symbol == DOLLAR)
                success = true;
            else
            {
                // printf("%d",parser->scanner->current_token->type);
                // printf("%d", actual_symbol);
                // printf("false8\n");
                FREE_STACK(false);
            }
            break;
        }

        if ((expr_stack_top(stack)->symbol != E_NONTERM) && (current_token->type == OPERATOR_LEFT_BRACKET))
        {
            count_brackets += 1;
        }
        if ((expr_stack_top(stack)->symbol != E_NONTERM) && (current_token->type == OPERATOR_RIGHT_BRACKET))
        {
            count_brackets -= 1;
            if (count_brackets < 0)
            {
                if (expr_stack_top(stack)->symbol != PLUS && expr_stack_top(stack)->symbol != MINUS &&
                    expr_stack_top(stack)->symbol != MUL && expr_stack_top(stack)->symbol != DIV &&
                    expr_stack_top(stack)->symbol != EQ && expr_stack_top(stack)->symbol != CONC &&
                    expr_stack_top(stack)->symbol != N_EQ && expr_stack_top(stack)->symbol != L_EQ &&
                    expr_stack_top(stack)->symbol != LESS && expr_stack_top(stack)->symbol != M_EQ &&
                    expr_stack_top(stack)->symbol != MORE)
                {
                    expr_stack_free(stack);
                    return true;
                }
                else
                {
                    expr_stack_free(stack);
                    // printf("khgv");
                    return false;
                }
            }
        }
    } while (!success);

    // //printf("%d", expr_stack_top(stack)->symbol);

    expr_stack_item_t *final_non_terminal = expr_stack_top(stack);
    if (final_non_terminal == NULL)
    {
        // printf("%d",parser->scanner->current_token->type);
        // printf("false9\n");
        FREE_STACK(false);
    }
    if (final_non_terminal->symbol != E_NONTERM)
    {
        // printf("%d",parser->scanner->current_token->type);
        // printf("false10\n");
        FREE_STACK(false);
    }
    // printf("%d",parser->scanner->current_token->type);
    // printf("true10\n");
    //  symbol_variable_t * arg = var->find_arg(var, variable_name);
    //                  arg->assign(arg, NULL, )
    symbol_variable_t *var = NULL;
    switch (expr_type)
    {
    case FUNC_VARIABLE:
        if ((var = st_get(st->frame_name)))
        {
            if (var->find_arg_g(var, variable_name) != retype_E(final_non_terminal->non_term_type))
            {
                exit_failure(SEM_RUN_ARGS_OR_RETURN_FUNC_ERR);
            }
        }
        else
        {
            exit_failure(SEMANTIC_UNDEFINED_VAR_ERR);
        }
        break;
    case ASSINGNMENT:
        if ((var = st_get(variable_name)))
        {
            var->assign(var, NULL, retype_E(final_non_terminal->non_term_type));
            frame_add_line(as DEFVAR(new_arg(var->type, var->name))); //создает переменную
            frame_add_line(as POPS(new_arg(TF, RESULT)));
        }
        else
        {
            exit_failure(SEMANTIC_UNDEFINED_VAR_ERR);
        }
        break;
    case RETURN:
        if (variable_name == NULL)
        {
            // return in main
        }
        else
        {
            if ((var = st_get(st->frame_name)))
            {
                if (var->type != retype_E(final_non_terminal->non_term_type))
                {
                    exit_failure(SEM_RUN_ARGS_OR_RETURN_FUNC_ERR);
                }
            }
            else
            {
                exit_failure(SEMANTIC_UNDEFINED_VAR_ERR);
            }
        }
        break;
    case CONDITION:
        break;

    case STATEMENT:
        break;
    }
    // st_debug;
    expr_stack_free(stack);
    return true;
}
