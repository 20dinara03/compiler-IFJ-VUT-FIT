/**
 * @file realscanner.c
 * @author xgarip00
 * @brief Real code scanner
 * @version 0.1
 */
#include "realscanner.h"

void Scan(FILE *file)
{
    /* Setting up default values for queues & tokens */

    queue_t *queue = init_queue();
    token_t *token = init_token();

    /*Reading 1 character from a file until we reach the end of it*/
    char newChar;
    while (!feof(file))
    {
        newChar = fgetc(file);

        switch (newChar)
        {
        case '\r':
        case '\n':
            if (token->type == COMMENT_LINE)
            {
                push_token_in_queue(queue, &token);
            }
            break;
        case '*':
            if (token->type == COMMENT)
            {
                push_char_in_token(token, newChar);
                token->type = POSSIBLE_COMMENT_END;
            }
            else if (token->type == POSSIBLE_COMMENT_END)
            {
                push_char_in_token(token, newChar);
            }
            else if (token->type == POSSIBLE_COMMENT)
            {
                push_char_in_token(token, newChar);
                token->type = COMMENT;
            }
            break;
        case '/':
            if (token->type == WHITE_SPACE)
            {
                push_char_in_token(token, newChar);
                token->type = POSSIBLE_COMMENT;
            }
            else if (token->type == POSSIBLE_COMMENT)
            {
                push_char_in_token(token, newChar);
                token->type = COMMENT_LINE;
            }
            else if (token->type == POSSIBLE_COMMENT_END)
            {
                push_char_in_token(token, newChar);
                token->type = COMMENT;
                push_token_in_queue(queue, &token);
            }
            break;
        default: 
            if (token->type == COMMENT || token->type == COMMENT_LINE)
            {
                push_char_in_token(token, newChar);
            }else if(token->type == POSSIBLE_COMMENT)
            {
                token->type = OPERATOR;
                push_token_in_queue(queue, &token);
            }
        }

        if (token->type == COMMENT ||
            token->type == COMMENT_LINE ||
            token->type == POSSIBLE_COMMENT_END ||
            token->type == POSSIBLE_COMMENT)
        {
            continue;
        }

        switch (newChar)
        {
        case '"':
            if (token->type == WHITE_SPACE)
            {
                token->type = STRING_LITERAL;
                push_char_in_token(token, newChar);
            }
            else if (token->type == STRING_LITERAL)
            {
                push_char_in_token(token, newChar);
                push_token_in_queue(queue, &token);
            }
            else
            {
                push_token_in_queue(queue, &token);
                push_char_in_token(token, newChar);
                token->type = STRING_LITERAL;
            }
            break;
        default:
            if(token->type == STRING_LITERAL){
                push_char_in_token(token, newChar);
                continue;
            }
        }

        // @TODO
        switch (newChar)
        {

            /*Possible reading variations for operands '+','-', ',' , '.', ':', '(', ')', '{', '}' */

        case '+':
        case '-':
        case '*':
        case '(':
        case ')':
        case ':':
        case ',':
        case '{':
        case '}':
        case ';':
        case '\\':

            /*If it's not in quotes or a comment,* then we create a new token for the operator and write it there*/
            if (token->type == WHITE_SPACE)
            {
                token->type = OPERATOR;
                push_char_in_token(token, newChar);
                push_token_in_queue(queue, &token);
            }
            /* @TODO */
            else if (token->type == EXPONENTA)
            {
                push_char_in_token(token, newChar);
                token->type = DOUBLE_LITERAL;
            }
            /*Otherwise, continue writing to the existing token*/
            else
            {
                push_token_in_queue(queue, &token);
                token->type = OPERATOR;
                push_char_in_token(token, newChar);
                push_token_in_queue(queue, &token);
            }
            break;
        case '.':
            if (token->type == WHITE_SPACE)
            {
                token->type = OPERATOR;
                push_char_in_token(token, newChar);
                push_token_in_queue(queue, &token);
            }
            else if (token->type == INT_LITERAL)
            {
                push_char_in_token(token, newChar);
                token->type = DOUBLE_LITERAL;
            }
            else
            {
                push_token_in_queue(queue, &token);
                token->type = OPERATOR;
                push_char_in_token(token, newChar);
                push_token_in_queue(queue, &token);
            }
            break;

            /*Possible reading variations for numbers*/
        case '0' ... '9':

            /*If it's not in quotes or a comment  then we create a new token for the INT literal and write it there*/

            if (token->type == WHITE_SPACE)
            {
                push_char_in_token(token, newChar);
                token->type = INT_LITERAL;
            }
            else if (token->type == INT_LITERAL ||
                     token->type == DOUBLE_LITERAL ||
                     token->type == EXPONENTA)
            {
                push_char_in_token(token, newChar);
            }
            /*Otherwise, continue writing to the existing token*/
            else
            {
                push_token_in_queue(queue, &token);
                push_char_in_token(token, newChar);
                token->type = INT_LITERAL;
            }
            break;

            /*Possible reading variations for operand '<'*/

        case '<':

            /*If it's not in quotes or a comment, then we create a new token for the operator or keyword '<?php' and write it there*/

            if (token->type == WHITE_SPACE)
            {
                token->type = POSSIBLE_KEYWORD;
                push_char_in_token(token, newChar);
            }
            /*Otherwise, continue writing to the existing token*/
            else
            {
                push_token_in_queue(queue, &token);
                token->type = POSSIBLE_KEYWORD;
                push_char_in_token(token, newChar);
            }
            break;

            /*Possible reading variations for operand '>'*/

        case '>':

            /*If it's not in quotes or a comment or a possible keyword, then we create a new token for the operator  and write it there*/

            if (token->type == WHITE_SPACE)
            {
                token->type = OPERATOR;
                push_char_in_token(token, newChar);
            }

            /*Otherwise if type of token is keyword, so write the operand to the token and save token with '?>' keyword */

            else if (token->type == KEYWORD)
            {
                push_char_in_token(token, newChar);
                push_token_in_queue(queue, &token);
            }

            /*Otherwise, continue writing to the existing token*/

            else
            {
                push_token_in_queue(queue, &token);
                token->type = OPERATOR;
                push_char_in_token(token, newChar);
            }
            break;

            /*Possible reading variations for key simbol '?'*/

        case '?':

            /*If it's not in quotes or a comment or a possible keyword '<?php', then we create a new token for the keyword '?>'  and write it there*/
            if (token->type == WHITE_SPACE)
            {
                token->type = KEYWORD;
                push_char_in_token(token, newChar);
            }
            else if (token->type == POSSIBLE_KEYWORD)
            {
                push_char_in_token(token, newChar);
            }

            /*Otherwise, continue writing to the existing token*/

            else
            {
                push_token_in_queue(queue, &token);
                token->type = POSSIBLE_KEYWORD;
                push_char_in_token(token, newChar);
            }
            break;

            /*Possible reading variations for key simbol '='*/

        case '=':

            /*If it's not in quotes or a comment or an operator '!' or '=' or '>', then we create a new token for the operator  and write it there*/

            if (token->type == WHITE_SPACE)
            {
                token->type = OPERATOR;
                push_char_in_token(token, newChar);
            }
            else if (token->type == POSSIBLE_KEYWORD)
            {
                push_char_in_token(token, newChar);
                token->type = OPERATOR;
                push_token_in_queue(queue, &token);
            }
            else if (token->type == OPERATOR)
            {
                push_char_in_token(token, newChar);
            }
            /*Otherwise, continue writing to the existing token*/

            else
            {
                push_token_in_queue(queue, &token);
                token->type = OPERATOR;
                push_char_in_token(token, newChar);
            }
            break;

            /*Possible reading variations for key simbol '!'*/

        case '!':

            /*If it's not in quotes or a comment, then we create a new token for the operator  and write it there*/

            if (token->type == WHITE_SPACE)
            {
                token->type = OPERATOR;
                push_char_in_token(token, newChar);
            }

            /*Otherwise, continue writing to the existing token*/

            else
            {
                push_token_in_queue(queue, &token);
                token->type = OPERATOR;
                push_char_in_token(token, newChar);
            }
            break;

            /*Possible reading variations for key simbol '|'*/

        case '|':

            /*If it's not in quotes or a comment or a token type is not possible OR, then we create a new token for the operator  and write it there*/

            if (token->type == WHITE_SPACE)
            {
                token->type = POSSIBLE_OR;
                push_char_in_token(token, newChar);
            }

            /*Otherwise if type of token is possible or, so write the operand to the token and save token with '||' operator */

            else if (token->type == POSSIBLE_OR)
            {
                push_char_in_token(token, newChar);
                token->type = OPERATOR;
                push_token_in_queue(queue, &token);
            }

            /*Otherwise, continue writing to the existing token*/

            else
            {
                push_token_in_queue(queue, &token);
                token->type = OPERATOR;
                push_char_in_token(token, newChar);
            }
            break;

            /*Possible reading variations for key simbol '&'*/

        case '&':

            /*If it's not in quotes or a comment or a token type is not possible AND, then we create a new token for the operator  and write it there*/
            if (token->type == WHITE_SPACE)
            {
                token->type = POSSIBLE_AND;
                push_char_in_token(token, newChar);
            }
            /*Otherwise if type of token is possible and, so write the operand to the token and save token with '||' operator */

            else if (token->type == POSSIBLE_AND)
            {
                push_char_in_token(token, newChar);
                token->type = OPERATOR;
                push_token_in_queue(queue, &token);
            }
            /*Otherwise, continue writing to the existing token*/

            else
            {
                push_token_in_queue(queue, &token);
                token->type = POSSIBLE_AND;
                push_char_in_token(token, newChar);
            }
            break;

            /*Possible reading variations for key simbol '/'*/
        case '\n':
        case '\r':
            if (token->type != WHITE_SPACE)
            {
                push_token_in_queue(queue, &token);
            }
            break;
        case ' ':
        case '\t':
            if (token->type != WHITE_SPACE)
            {
                push_token_in_queue(queue, &token);
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
                push_char_in_token(token, newChar);
                token->type = IDENTIFIER;
            }
            else if (token->type == KEYWORD ||
                     token->type == IDENTIFIER)
            {
                push_char_in_token(token, newChar);
            }
            else
            {
                push_token_in_queue(queue, &token);
                token->type = IDENTIFIER;
                push_char_in_token(token, newChar);
            }
            break;
        case '$': //@TODO remove $  -> $aa$aa
        case '_':
            if (token->type == WHITE_SPACE)
            {
                token->type = IDENTIFIER;
                push_char_in_token(token, newChar);
            }
            else if (token->type == IDENTIFIER)
            {
                push_char_in_token(token, newChar);
            }
            else
            {
                push_token_in_queue(queue, &token);
                push_char_in_token(token, newChar);
                token->type = IDENTIFIER;
            }
            break;

        case 'e':
        case 'E':
            if (token->type == WHITE_SPACE)
            {
                push_char_in_token(token, newChar);
                token->type = IDENTIFIER;
            }
            else if (token->type == IDENTIFIER)
            {
                push_char_in_token(token, newChar);
            }
            else if (token->type == DOUBLE_LITERAL)
            {
                push_char_in_token(token, newChar);
                token->type = EXPONENTA;
            }
            else
            {
                push_token_in_queue(queue, &token);
                push_char_in_token(token, newChar);
                token->type = IDENTIFIER;
            }
            break;
        case 'p':
            if (token->type == WHITE_SPACE)
            {
                token->type = IDENTIFIER;
                push_char_in_token(token, newChar);
            }
            else if (token->type == POSSIBLE_KEYWORD)
            {
                push_char_in_token(token, newChar);
                token->type = KEYWORD_p;
            }
            else if (token->type == IDENTIFIER ||
                     token->type == KEYWORD)
            {
                push_char_in_token(token, newChar);
            }
            else if (token->type == KEYWORD_h)
            {
                push_char_in_token(token, newChar);
                token->type = KEYWORD;
                push_token_in_queue(queue, &token);
            }
            else
            {
                push_token_in_queue(queue, &token);
                push_char_in_token(token, newChar);
                token->type = IDENTIFIER;
            }
            break;

        case 'h':
            if (token->type == WHITE_SPACE)
            {
                push_char_in_token(token, newChar);
                token->type = IDENTIFIER;
            }
            else if (
                token->type == IDENTIFIER ||
                token->type == KEYWORD)
            {
                push_char_in_token(token, newChar);
            }
            else if (token->type == KEYWORD_p)
            {
                push_char_in_token(token, newChar);
                token->type = KEYWORD_h;
            }
            else
            {
                push_token_in_queue(queue, &token);
                push_char_in_token(token, newChar);
                token->type = IDENTIFIER;
            }
            break;
        }
    }
    node_t *node = queue->head;

    while (node)
    {
        print_token(node->data);
        node = node->next;
    }

    /* clean up */

    free_token(token);
    free_queue(queue, free_token);
}
