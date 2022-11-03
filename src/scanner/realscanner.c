/**
 * @file realscanner.c
 * @author xgarip00
 * @brief Real code scanner
 * @version 0.1
 */
#include "realscanner.h"
#include "input_stack.h"

void Scan(FILE *file)
{
    /* Setting up default values for stacks & tokens */

    input_stack_t *stack = init_input_stack();
    token_t *token = init_token();

    /*Reading 1 character from a file until we reach the end of it*/
    char current_char;
    while (!feof(file))
    {
        current_char = fgetc(file);

        switch (current_char)
        {
        case '\r':
        case '\n':
            if (token->type == COMMENT_LINE)
            {
                stack->push(stack, token);
                token->reset(&token);
            }
            break;
        case '*':
            if (token->type == COMMENT)
            {
                token->push_char(token, current_char);
                token->type = POSSIBLE_COMMENT_END;
            }
            else if (token->type == POSSIBLE_COMMENT_END)
            {
                token->push_char(token, current_char);
            }
            else if (token->type == POSSIBLE_COMMENT)
            {
                token->push_char(token, current_char);
                token->type = COMMENT;
            }
            break;
        case '/':
            if (token->type == WHITE_SPACE)
            {
                token->push_char(token, current_char);
                token->type = POSSIBLE_COMMENT;
            }
            else if (token->type == POSSIBLE_COMMENT)
            {
                token->push_char(token, current_char);
                token->type = COMMENT_LINE;
            }
            else if (token->type == POSSIBLE_COMMENT_END)
            {
                token->push_char(token, current_char);
                token->type = COMMENT;
                stack->push(stack, token);
                token->reset(&token);
            }
            break;
        default:
            if (token->type == COMMENT || token->type == COMMENT_LINE)
            {
                token->push_char(token, current_char);
            }
            else if (token->type == POSSIBLE_COMMENT)
            {
                token->type = OPERATOR_DIVIDE;
                stack->push(stack, token);
                token->reset(&token);
            }
        }

        if (token->type == COMMENT ||
            token->type == COMMENT_LINE ||
            token->type == POSSIBLE_COMMENT_END ||
            token->type == POSSIBLE_COMMENT)
        {
            continue;
        }

        switch (current_char)
        {
        case '"':
            if (token->type == WHITE_SPACE)
            {
                token->type = STRING_LITERAL;
                token->push_char(token, current_char);
            }
            else if (token->type == STRING_LITERAL)
            {
                token->push_char(token, current_char);
                stack->push(stack, token);
                token->reset(&token);
            }
            else
            {
                stack->push(stack, token);
                token->reset(&token);
                token->push_char(token, current_char);
                token->type = STRING_LITERAL;
            }
            break;
        default:
            if (token->type == STRING_LITERAL)
            {
                token->push_char(token, current_char);
                continue;
            }
        }

        // @TODO
        switch (current_char)
        {

            /*Possible reading variations for operands '+','-', ',' , '.', ':', '(', ')', '{', '}' */

        case '+':
             /*If it's not in quotes or a comment,* then we create a new token for the operator and write it there*/
            if (token->type == WHITE_SPACE)
            {
                token->type = OPERATOR_PLUS;
                token->push_char(token, current_char);
                stack->push(stack, token);
                token->reset(&token);
            }
            else if (token->type == EXPONENTA)
            {
                token->push_char(token, current_char);
                token->type = DOUBLE_LITERAL;
            }
            /*Otherwise, continue writing to the existing token*/
            else
            {
                stack->push(stack, token);
                token->reset(&token);
                token->type = OPERATOR_PLUS;
                token->push_char(token, current_char);
                stack->push(stack, token);
                token->reset(&token);
            }
            break;
        case '-':
             /*If it's not in quotes or a comment,* then we create a new token for the operator and write it there*/
            if (token->type == WHITE_SPACE)
            {
                token->type = OPERATOR_MINUS;
                token->push_char(token, current_char);
                stack->push(stack, token);
                token->reset(&token);
            }
            else if (token->type == EXPONENTA)
            {
                token->push_char(token, current_char);
                token->type = DOUBLE_LITERAL;
            }
            /*Otherwise, continue writing to the existing token*/
            else
            {
                stack->push(stack, token);
                token->reset(&token);
                token->type = OPERATOR_MINUS;
                token->push_char(token, current_char);
                stack->push(stack, token);
                token->reset(&token);
            }
            break;
        case '*':
            /*If it's not in quotes or a comment,* then we create a new token for the operator and write it there*/
            if (token->type == WHITE_SPACE)
            {
                token->type = OPERATOR_MULTIPLY;
                token->push_char(token, current_char);
                stack->push(stack, token);
                token->reset(&token);
            }
            /*Otherwise, continue writing to the existing token*/
            else
            {
                stack->push(stack, token);
                token->reset(&token);
                token->type = OPERATOR_MULTIPLY;
                token->push_char(token, current_char);
                stack->push(stack, token);
                token->reset(&token);
            }
            break;
        case '(':
            /*If it's not in quotes or a comment,* then we create a new token for the operator and write it there*/
            if (token->type == WHITE_SPACE)
            {
                token->type = OPERATOR_LEFT_BRACKET;
                token->push_char(token, current_char);
                stack->push(stack, token);
                token->reset(&token);
            }
            /*Otherwise, continue writing to the existing token*/
            else
            {
                stack->push(stack, token);
                token->reset(&token);
                token->type = OPERATOR_LEFT_BRACKET;
                token->push_char(token, current_char);
                stack->push(stack, token);
                token->reset(&token);
            }
            break;
        case ')':
            /*If it's not in quotes or a comment,* then we create a new token for the operator and write it there*/
            if (token->type == WHITE_SPACE)
            {
                token->type = OPERATOR_RIGHT_BRACKET;
                token->push_char(token, current_char);
                stack->push(stack, token);
                token->reset(&token);
            }
            /*Otherwise, continue writing to the existing token*/
            else
            {
                stack->push(stack, token);
                token->reset(&token);
                token->type = OPERATOR_RIGHT_BRACKET;
                token->push_char(token, current_char);
                stack->push(stack, token);
                token->reset(&token);
            }
            break;
        case ':':
            /*If it's not in quotes or a comment,* then we create a new token for the operator and write it there*/
            if (token->type == WHITE_SPACE)
            {
                token->type = OPERATOR_COLON;
                token->push_char(token, current_char);
                stack->push(stack, token);
                token->reset(&token);
            }
            /*Otherwise, continue writing to the existing token*/
            else
            {
                stack->push(stack, token);
                token->reset(&token);
                token->type = OPERATOR_COLON;
                token->push_char(token, current_char);
                stack->push(stack, token);
                token->reset(&token);
            }
            break;
        case ',':
            /*If it's not in quotes or a comment,* then we create a new token for the operator and write it there*/
            if (token->type == WHITE_SPACE)
            {
                token->type = OPERATOR_COMMA;
                token->push_char(token, current_char);
                stack->push(stack, token);
                token->reset(&token);
            }
            /*Otherwise, continue writing to the existing token*/
            else
            {
                stack->push(stack, token);
                token->reset(&token);
                token->type = OPERATOR_COMMA;
                token->push_char(token, current_char);
                stack->push(stack, token);
                token->reset(&token);
            }
            break;
        case '{':
            /*If it's not in quotes or a comment,* then we create a new token for the operator and write it there*/
            if (token->type == WHITE_SPACE)
            {
                token->type = OPERATOR_LEFT_CURLY_BRACKET;
                token->push_char(token, current_char);
                stack->push(stack, token);
                token->reset(&token);
            }
            /*Otherwise, continue writing to the existing token*/
            else
            {
                stack->push(stack, token);
                token->reset(&token);
                token->type = OPERATOR_LEFT_CURLY_BRACKET;
                token->push_char(token, current_char);
                stack->push(stack, token);
                token->reset(&token);
            }
            break;
        case '}':
            /*If it's not in quotes or a comment,* then we create a new token for the operator and write it there*/
            if (token->type == WHITE_SPACE)
            {
                token->type = OPERATOR_RIGHT_CURLY_BRACKET;
                token->push_char(token, current_char);
                stack->push(stack, token);
                token->reset(&token);
            }
            /*Otherwise, continue writing to the existing token*/
            else
            {
                stack->push(stack, token);
                token->reset(&token);
                token->type = OPERATOR_RIGHT_CURLY_BRACKET;
                token->push_char(token, current_char);
                stack->push(stack, token);
                token->reset(&token);
            }
            break;
        case ';':
            /*If it's not in quotes or a comment,* then we create a new token for the operator and write it there*/
            if (token->type == WHITE_SPACE)
            {
                token->type = OPERATOR_SEMICOLON;
                token->push_char(token, current_char);
                stack->push(stack, token);
                token->reset(&token);
            }
            /*Otherwise, continue writing to the existing token*/
            else
            {
                stack->push(stack, token);
                token->reset(&token);
                token->type = OPERATOR_SEMICOLON;
                token->push_char(token, current_char);
                stack->push(stack, token);
                token->reset(&token);
            }
            break;
        case '\\':

            /*If it's not in quotes or a comment,* then we create a new token for the operator and write it there*/
            if (token->type == WHITE_SPACE)
            {
                token->type = OPERATOR;
                token->push_char(token, current_char);
                stack->push(stack, token);
                token->reset(&token);
            }
            /*Otherwise, continue writing to the existing token*/
            else
            {
                stack->push(stack, token);
                token->reset(&token);
                token->type = OPERATOR;
                token->push_char(token, current_char);
                stack->push(stack, token);
                token->reset(&token);
            }
            break;
        case '.':
            if (token->type == WHITE_SPACE)
            {
                token->type = OPERATOR_POINT;
                token->push_char(token, current_char);
                stack->push(stack, token);
                token->reset(&token);
            }
            else if (token->type == INT_LITERAL)
            {
                token->push_char(token, current_char);
                token->type = DOUBLE_LITERAL;
            }
            else
            {
                stack->push(stack, token);
                token->reset(&token);
                token->type = OPERATOR_POINT;
                token->push_char(token, current_char);
                stack->push(stack, token);
                token->reset(&token);
            }
            break;

            /*Possible reading variations for numbers*/
        case '0' ... '9':

            /*If it's not in quotes or a comment  then we create a new token for the INT literal and write it there*/

            if (token->type == WHITE_SPACE)
            {
                token->push_char(token, current_char);
                token->type = INT_LITERAL;
            }
            else if (token->type == INT_LITERAL ||
                     token->type == DOUBLE_LITERAL ||
                     token->type == EXPONENTA)
            {
                token->push_char(token, current_char);
            }
            /*Otherwise, continue writing to the existing token*/
            else
            {
                stack->push(stack, token);
                token->reset(&token);
                token->push_char(token, current_char);
                token->type = INT_LITERAL;
            }
            break;

            /*Possible reading variations for operand '<'*/

        case '<':

            /*If it's not in quotes or a comment, then we create a new token for the operator or keyword '<?php' and write it there*/

            if (token->type == WHITE_SPACE)
            {
                token->type = OPERATOR_LESS;
                token->push_char(token, current_char);
            }
            /*Otherwise, continue writing to the existing token*/
            else
            {
                stack->push(stack, token);
                token->reset(&token);
                token->type = OPERATOR_LESS;
                token->push_char(token, current_char);
            }
            break;

            /*Possible reading variations for operand '>'*/

        case '>':

            /*If it's not in quotes or a comment or a possible keyword, then we create a new token for the operator  and write it there*/

            if (token->type == WHITE_SPACE)
            {
                token->type = OPERATOR_GREATER;
                token->push_char(token, current_char);
            }

            /*Otherwise if type of token is keyword, so write the operand to the token and save token with '?>' keyword */

            else if (token->type == KEYWORD)
            {
                token->push_char(token, current_char);
                stack->push(stack, token);
                token->reset(&token);
            }

            /*Otherwise, continue writing to the existing token*/

            else
            {
                stack->push(stack, token);
                token->reset(&token);
                token->type = OPERATOR_GREATER;
                token->push_char(token, current_char);
            }
            break;

            /*Possible reading variations for key simbol '?'*/

        case '?':

            /*If it's not in quotes or a comment or a possible keyword '<?php', then we create a new token for the keyword '?>'  and write it there*/
            if (token->type == WHITE_SPACE)
            {
                token->type = KEYWORD;
                token->push_char(token, current_char);
            }
            else if (token->type == OPERATOR_LESS)
            {
                token->push_char(token, current_char);
                token->type = POSSIBLE_KEYWORD;
            }

            /*Otherwise, continue writing to the existing token*/

            else
            {
                stack->push(stack, token);
                token->reset(&token);
                token->type = POSSIBLE_KEYWORD;
                token->push_char(token, current_char);
            }
            break;

            /*Possible reading variations for key simbol '='*/

        case '=':

            /*If it's not in quotes or a comment or an operator '!' or '=' or '>', then we create a new token for the operator  and write it there*/

            if (token->type == WHITE_SPACE)
            {
                token->type = OPERATOR_EQUAL;
                token->push_char(token, current_char);
            }
            else if (token->type == OPERATOR_LESS)
            {
                token->push_char(token, current_char);
                token->type = OPERATOR_LESS_OR_EQUAL;
                stack->push(stack, token);
                token->reset(&token);
            }
            else if (token->type == OPERATOR_GREATER)
            {
                token->push_char(token, current_char);
                token->type = OPERATOR_GREATER_OR_EQUAL;
                stack->push(stack, token);
                token->reset(&token);
            }
            else if (token->type == OPERATOR_NOT)
            {
                token->push_char(token, current_char);
                token->type = OPERATOR_NOT_EQUAL;
                stack->push(stack, token);
                token->reset(&token);
            }
            else if (token->type == OPERATOR_EQUAL)
            {
                token->push_char(token, current_char);
            }
            /*Otherwise, continue writing to the existing token*/

            else
            {
                stack->push(stack, token);
                token->reset(&token);
                token->type = OPERATOR_EQUAL;
                token->push_char(token, current_char);
            }
            break;

            /*Possible reading variations for key simbol '!'*/

        case '!':

            /*If it's not in quotes or a comment, then we create a new token for the operator  and write it there*/

            if (token->type == WHITE_SPACE)
            {
                token->type = OPERATOR_NOT;
                token->push_char(token, current_char);
            }

            /*Otherwise, continue writing to the existing token*/

            else
            {
                stack->push(stack, token);
                token->reset(&token);
                token->type = OPERATOR_NOT;
                token->push_char(token, current_char);
            }
            break;

            /*Possible reading variations for key simbol '|'*/

        case '|':

            /*If it's not in quotes or a comment or a token type is not possible OR, then we create a new token for the operator  and write it there*/

            if (token->type == WHITE_SPACE)
            {
                token->type = POSSIBLE_OR;
                token->push_char(token, current_char);
            }

            /*Otherwise if type of token is possible or, so write the operand to the token and save token with '||' operator */

            else if (token->type == POSSIBLE_OR)
            {
                token->push_char(token, current_char);
                token->type = OPERATOR_OR;
                stack->push(stack, token);
                token->reset(&token);
            }

            /*Otherwise, continue writing to the existing token*/

            else
            {
                stack->push(stack, token);
                token->reset(&token);
                token->type = POSSIBLE_OR;
                token->push_char(token, current_char);
            }
            break;

            /*Possible reading variations for key simbol '&'*/

        case '&':

            /*If it's not in quotes or a comment or a token type is not possible AND, then we create a new token for the operator  and write it there*/
            if (token->type == WHITE_SPACE)
            {
                token->type = POSSIBLE_AND;
                token->push_char(token, current_char);
            }
            /*Otherwise if type of token is possible and, so write the operand to the token and save token with '||' operator */

            else if (token->type == POSSIBLE_AND)
            {
                token->push_char(token, current_char);
                token->type = OPERATOR_AND;
                stack->push(stack, token);
                token->reset(&token);
            }
            /*Otherwise, continue writing to the existing token*/

            else
            {
                stack->push(stack, token);
                token->reset(&token);
                token->type = POSSIBLE_AND;
                token->push_char(token, current_char);
            }
            break;

            /*Possible reading variations for key simbol '/'*/
        case '\n':
        case '\r':
            if (token->type != WHITE_SPACE)
            {
                stack->push(stack, token);
                token->reset(&token);
            }
            break;
        case ' ':
        case '\t':
            if (token->type != WHITE_SPACE)
            {
                stack->push(stack, token);
                token->reset(&token);
            }
            break;
        case 'A' ... 'D':
        case 'F' ... 'Z':
        case 'a' ... 'd':
        case 'f' ... 'g':
        case 'i' ... 'o':
        case 'q' ... 'z':
            if (token->type == WHITE_SPACE)
            {
                token->push_char(token, current_char);
                token->type = IDENTIFIER;
            }
            else if (token->type == KEYWORD ||
                     token->type == IDENTIFIER)
            {
                token->push_char(token, current_char);
            }
            else
            {
                stack->push(stack, token);
                token->reset(&token);
                token->type = IDENTIFIER;
                token->push_char(token, current_char);
            }
            break;
        case '$': //@TODO remove $  -> $aa$aa
        case '_':
            if (token->type == WHITE_SPACE)
            {
                token->type = IDENTIFIER;
                token->push_char(token, current_char);
            }
            else if (token->type == IDENTIFIER)
            {
                token->push_char(token, current_char);
            }
            else
            {
                stack->push(stack, token);
                token->reset(&token);
                token->push_char(token, current_char);
                token->type = IDENTIFIER;
            }
            break;

        case 'e':
        case 'E':
            if (token->type == WHITE_SPACE)
            {
                token->push_char(token, current_char);
                token->type = IDENTIFIER;
            }
            else if (token->type == IDENTIFIER)
            {
                token->push_char(token, current_char);
            }
            else if (token->type == DOUBLE_LITERAL)
            {
                token->push_char(token, current_char);
                token->type = EXPONENTA;
            }
            else
            {
                stack->push(stack, token);
                token->reset(&token);
                token->push_char(token, current_char);
                token->type = IDENTIFIER;
            }
            break;
        case 'p':
            if (token->type == WHITE_SPACE)
            {
                token->type = IDENTIFIER;
                token->push_char(token, current_char);
            }
            else if (token->type == POSSIBLE_KEYWORD)
            {
                token->push_char(token, current_char);
                token->type = KEYWORD_p;
            }
            else if (token->type == IDENTIFIER ||
                     token->type == KEYWORD)
            {
                token->push_char(token, current_char);
            }
            else if (token->type == KEYWORD_h)
            {
                token->push_char(token, current_char);
                token->type = KEYWORD;
                stack->push(stack, token);
                token->reset(&token);
            }
            else
            {
                stack->push(stack, token);
                token->reset(&token);
                token->push_char(token, current_char);
                token->type = IDENTIFIER;
            }
            break;

        case 'h':
            if (token->type == WHITE_SPACE)
            {
                token->push_char(token, current_char);
                token->type = IDENTIFIER;
            }
            else if (
                token->type == IDENTIFIER ||
                token->type == KEYWORD)
            {
                token->push_char(token, current_char);
            }
            else if (token->type == KEYWORD_p)
            {
                token->push_char(token, current_char);
                token->type = KEYWORD_h;
            }
            else
            {
                stack->push(stack, token);
                token->reset(&token);
                token->push_char(token, current_char);
                token->type = IDENTIFIER;
            }
            break;
        }
    }
    input_node_t *node = stack->head;

    while (node)
    {
        node->token->debug(node->token);
        node = node->next;
    }

    /* clean up */
    stack->free(&stack);
    token->free(&token);
}
