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

            /*Possible reading variations for operands '+','-', ',' , '.', ':', '(', ')', '{', '}' */

        case '(':
        case ')':
        case '+':
        case ':':
        case ',':
        case '-':
        case '.':
        case '{':
        case '}':
        case ';':
            /*If it's not in quotes or a comment, then we create a new token for the operator and write it there*/

            if (token->type != STRING_LITERAL &&
                token->type != COMMENT &&
                token->type != POSSIBLE_COMMENT_END &&
                token->type != COMMENT_LINE &&
                token->type != WHITE_SPACE)
            {
                push_token_in_queue(queue, &token);
                token->type = OPERATOR;
                push_char_in_token(token, newChar);
                push_token_in_queue(queue, &token);
            }
            else if (token->type == POSSIBLE_COMMENT_END)
            {
                push_char_in_token(token, newChar);
                token->type = COMMENT;
            }
            else if (token->type == WHITE_SPACE)
            {
                token->type = OPERATOR;
                push_char_in_token(token, newChar);
                push_token_in_queue(queue, &token);
            }

            /*Otherwise, continue writing to the existing token*/

            else
            {
                push_char_in_token(token, newChar);
            }
            break;

            /*Possible reading variations for numbers*/

        case '0' ... '9':

            /*If it's not in quotes or a comment  then we create a new token for the INT literal and write it there*/

            if (token->type != STRING_LITERAL &&
                token->type != COMMENT &&
                token->type != POSSIBLE_COMMENT_END &&
                token->type != COMMENT_LINE &&
                token->type != WHITE_SPACE)
            {
                push_token_in_queue(queue, &token);
                push_char_in_token(token, newChar);
                token->type = INT_LITERAL;
            }
            else if (token->type == POSSIBLE_COMMENT_END)
            {
                push_char_in_token(token, newChar);
                token->type = COMMENT;
            }
            else if (token->type == WHITE_SPACE)
            {
                push_char_in_token(token, newChar);
                token->type = INT_LITERAL;
            }

            /*Otherwise, continue writing to the existing token*/

            else
            {
                push_char_in_token(token, newChar);
            }
            break;

            /*Possible reading variations for operand '<'*/

        case '<':

            /*If it's not in quotes or a comment, then we create a new token for the operator or keyword '<?php' and write it there*/

            if (token->type != STRING_LITERAL &&
                token->type != COMMENT &&
                token->type != POSSIBLE_COMMENT_END &&
                token->type != COMMENT_LINE &&
                token->type != WHITE_SPACE)
            {
                push_token_in_queue(queue, &token);
                token->type = POSSIBLE_KEYWORD;
                push_char_in_token(token, newChar);
            }
            else if (token->type == POSSIBLE_COMMENT_END)
            {
                push_char_in_token(token, newChar);
                token->type = COMMENT;
            }
            else if (token->type == WHITE_SPACE)
            {
                token->type = POSSIBLE_KEYWORD;
                push_char_in_token(token, newChar);
            }


            /*Otherwise, continue writing to the existing token*/

            else
            {
                push_char_in_token(token, newChar);
            }
            break;

            /*Possible reading variations for operand '>'*/

        case '>':

            /*If it's not in quotes or a comment or a possible keyword, then we create a new token for the operator  and write it there*/

            if (token->type != STRING_LITERAL &&
                token->type != COMMENT &&
                token->type != KEYWORD &&
                token->type != POSSIBLE_COMMENT_END &&
                token->type != COMMENT_LINE &&
                token->type != WHITE_SPACE)
            {
                push_token_in_queue(queue, &token);
                token->type = OPERATOR;
                push_char_in_token(token, newChar);
            }
            else if (token->type == POSSIBLE_COMMENT_END)
            {
                push_char_in_token(token, newChar);
                token->type = COMMENT;
            }
            else if (token->type == WHITE_SPACE)
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
                push_char_in_token(token, newChar);
            }
            break;

            /*Possible reading variations for key simbol '?'*/

        case '?':

            /*If it's not in quotes or a comment or a possible keyword '<?php', then we create a new token for the keyword '?>'  and write it there*/

            if (token->type != STRING_LITERAL &&
                token->type != COMMENT &&
                token->type != POSSIBLE_KEYWORD &&
                token->type != POSSIBLE_COMMENT_END &&
                token->type != COMMENT_LINE &&
                token->type != WHITE_SPACE)
            {
                push_token_in_queue(queue, &token);
                token->type = KEYWORD;
                push_char_in_token(token, newChar);
            }
            else if (token->type == POSSIBLE_COMMENT_END)
            {
                push_char_in_token(token, newChar);
                token->type = COMMENT;
            }
            else if (token->type == WHITE_SPACE)
            {
                token->type = KEYWORD;
                push_char_in_token(token, newChar);
            }

            /*Otherwise, continue writing to the existing token*/

            else
            {
                push_char_in_token(token, newChar);
            }
            break;

            /*Possible reading variations for key simbol '='*/

        case '=':

            /*If it's not in quotes or a comment or an operator '!' or '=' or '>', then we create a new token for the operator  and write it there*/

            if (token->type != STRING_LITERAL &&
                token->type != COMMENT &&
                token->type != OPERATOR &&
                token->type != POSSIBLE_COMMENT_END &&
                token->type != COMMENT_LINE &&
                token->type != POSSIBLE_KEYWORD &&
                token->type != WHITE_SPACE)
            {
                push_token_in_queue(queue, &token);
                token->type = OPERATOR;
                push_char_in_token(token, newChar);
            }
            else if (token->type == POSSIBLE_COMMENT_END)
            {
                push_char_in_token(token, newChar);
                token->type = COMMENT;
            }
            else if (token->type == POSSIBLE_KEYWORD)
            {
              push_char_in_token(token, newChar);
              token->type = OPERATOR;
              push_token_in_queue(queue, &token);
            }
            else if (token->type == WHITE_SPACE)
            {
                token->type = OPERATOR;
                push_char_in_token(token, newChar);
            }
            /*Otherwise, continue writing to the existing token*/

            else
            {
                push_char_in_token(token, newChar);
            }
            break;

            /*Possible reading variations for key simbol '!'*/

        case '!':

            /*If it's not in quotes or a comment, then we create a new token for the operator  and write it there*/

            if (token->type != STRING_LITERAL &&
                token->type != COMMENT &&
                token->type != POSSIBLE_COMMENT_END &&
                token->type != COMMENT_LINE &&
                token->type != WHITE_SPACE)
            {
                push_token_in_queue(queue, &token);
                token->type = OPERATOR;
                push_char_in_token(token, newChar);
            }
            else if (token->type == POSSIBLE_COMMENT_END)
            {
                push_char_in_token(token, newChar);
                token->type = COMMENT;
            }
            else if (token->type == WHITE_SPACE)
            {
                token->type = OPERATOR;
                push_char_in_token(token, newChar);
            }

            /*Otherwise, continue writing to the existing token*/

            else
            {
                push_char_in_token(token, newChar);
            }
            break;

            /*Possible reading variations for key simbol '|'*/

        case '|':

            /*If it's not in quotes or a comment or a token type is not possible OR, then we create a new token for the operator  and write it there*/

            if (token->type != STRING_LITERAL &&
                token->type != COMMENT &&
                token->type != POSSIBLE_OR &&
                token->type != POSSIBLE_COMMENT_END &&
                token->type != COMMENT_LINE &&
                token->type != WHITE_SPACE)
            {
                push_token_in_queue(queue, &token);
                token->type = POSSIBLE_OR;
                push_char_in_token(token, newChar);
            }
            else if (token->type == POSSIBLE_COMMENT_END)
            {
                push_char_in_token(token, newChar);
                token->type = COMMENT;
            }
            else if (token->type == WHITE_SPACE)
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
                push_char_in_token(token, newChar);
            }
            break;

            /*Possible reading variations for key simbol '&'*/

        case '&':

            /*If it's not in quotes or a comment or a token type is not possible AND, then we create a new token for the operator  and write it there*/

            if (token->type != STRING_LITERAL &&
                token->type != COMMENT &&
                token->type != POSSIBLE_AND &&
                token->type != POSSIBLE_COMMENT_END &&
                token->type != COMMENT_LINE &&
                token->type != WHITE_SPACE)
            {
                push_token_in_queue(queue, &token);
                token->type = POSSIBLE_AND;
                push_char_in_token(token, newChar);
            }
            else if (token->type == POSSIBLE_COMMENT_END)
            {
                push_char_in_token(token, newChar);
                token->type = COMMENT;
            }
            else if (token->type == WHITE_SPACE)
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
                push_char_in_token(token, newChar);
            }
            break;

            /*Possible reading variations for key simbol '/'*/

        case '/':

            /*If it's not in quotes or a comment or a token type is not possible comment, then we create a new token for the operator  and write it there*/

            if (token->type != STRING_LITERAL &&
                token->type != COMMENT &&
                token->type != POSSIBLE_COMMENT &&
                token->type != POSSIBLE_COMMENT_END &&
                token->type != COMMENT_LINE &&
                token->type != WHITE_SPACE)
            {
                push_token_in_queue(queue, &token);
                token->type = POSSIBLE_COMMENT;
                push_char_in_token(token, newChar);
            }
            else if (token->type == WHITE_SPACE)
            {
                token->type = POSSIBLE_COMMENT;
                push_char_in_token(token, newChar);
            }

            /*If token type is possible comment then save operand to the token and push the type comment (there is comment starts)*/

            else if (token->type == POSSIBLE_COMMENT)
            {
                push_char_in_token(token, newChar);
                token->type = COMMENT_LINE;
            }

            /*If it is can be the end of the comment (befor was the operand '*'), so save the token with type comment */

            else if (token->type == POSSIBLE_COMMENT_END)
            {
                push_char_in_token(token, newChar);
                token->type = COMMENT;
                push_token_in_queue(queue, &token);
            }

            /*Otherwise, continue writing to the existing token*/

            else
            {
                push_char_in_token(token, newChar);
            }
            break;

            /*Possible reading variations for key simbol '*' */

        case '*':
            if (token->type != STRING_LITERAL &&
                token->type != COMMENT &&
                token->type != POSSIBLE_COMMENT &&
                token->type != WHITE_SPACE)
            {
                push_token_in_queue(queue, &token);
                token->type = OPERATOR;
                push_char_in_token(token, newChar);
                push_token_in_queue(queue, &token);
            }
            else if (token->type == WHITE_SPACE)
            {
                token->type = OPERATOR;
                push_char_in_token(token, newChar);
            }
            else if (token->type == POSSIBLE_COMMENT)
            {
                push_char_in_token(token, newChar);
                token->type = COMMENT;
            }
            else if (token->type == COMMENT)
            {
                push_char_in_token(token, newChar);
                token->type = POSSIBLE_COMMENT_END;
            }
            else
            {
                push_char_in_token(token, newChar);
            }
            break;
        case '\n':
            if (token->type == COMMENT_LINE)
            {
                push_token_in_queue(queue, &token);
            }
            break;
        case ' ':
        case '\t':

            if (token->type != STRING_LITERAL && 
                token->type != COMMENT &&
                token->type != COMMENT_LINE &&
                token->type != POSSIBLE_COMMENT_END)
            {
                push_token_in_queue(queue, &token);
            }
            else if (token->type == POSSIBLE_COMMENT_END)
            {
                push_char_in_token(token, newChar);
                token->type = COMMENT;
            }
            else
            {
              push_char_in_token(token, newChar);
            }
        break;
        case 'A'...'Z':
        case 'a'...'g':
        case 'i'...'o':
        case 'q'...'z':
        case '$':
        case '_':
          if (token->type != STRING_LITERAL &&
              token->type != COMMENT &&
              token->type != POSSIBLE_COMMENT_END &&
              token->type != COMMENT_LINE &&
              token->type != IDENTIFIER &&
                token->type != WHITE_SPACE)
            {
               push_token_in_queue(queue, &token);
               push_char_in_token(token, newChar);
               token->type = IDENTIFIER;
            }
            else if (token->type == POSSIBLE_COMMENT_END)
            {
                push_char_in_token(token, newChar);
                token->type = COMMENT;
            }
            else if (token->type == WHITE_SPACE)
            {
                token->type = IDENTIFIER;
                push_char_in_token(token, newChar);
            }
            else
            {
              push_char_in_token(token, newChar);
            }
        break;
        case 'p':
            if (token->type != STRING_LITERAL &&
                token->type != COMMENT &&
                token->type != POSSIBLE_COMMENT_END &&
                token->type != COMMENT_LINE &&
                token->type != IDENTIFIER &&
                token->type != POSSIBLE_KEYWORD &&
                token->type != KEYWORD_h &&
                token->type != WHITE_SPACE)
            {
               push_token_in_queue(queue, &token);
               push_char_in_token(token, newChar);
               token->type = IDENTIFIER;
            }
            else if (token->type == POSSIBLE_KEYWORD)
            {
               push_char_in_token(token, newChar);
               token->type = KEYWORD_p;
            }
            else if (token->type == WHITE_SPACE)
            {
                token->type = IDENTIFIER;
                push_char_in_token(token, newChar);
            }
            else if (token->type == KEYWORD_h)
            {
               push_char_in_token(token, newChar);
               token->type = KEYWORD;
               push_token_in_queue(queue, &token);
            }
            else if (token->type == POSSIBLE_COMMENT_END)
            {
                push_char_in_token(token, newChar);
                token->type = COMMENT;
            }
            else
            {
              push_char_in_token(token, newChar);
            }
        break;
        case 'h':
          if (token->type != STRING_LITERAL &&
                token->type != COMMENT &&
                token->type != POSSIBLE_COMMENT_END &&
                token->type != COMMENT_LINE &&
                token->type != IDENTIFIER &&
                token->type != KEYWORD_p &&
                token->type != WHITE_SPACE)
            {
               push_token_in_queue(queue, &token);
               push_char_in_token(token, newChar);
               token->type = IDENTIFIER;
            }
            else if (token->type == WHITE_SPACE)
            {
                token->type = IDENTIFIER;
                push_char_in_token(token, newChar);
            }
            else if (token->type == KEYWORD_p)
            {
               push_char_in_token(token, newChar);
               token->type = KEYWORD_h;
            }
            else if (token->type == POSSIBLE_COMMENT_END)
            {
                push_char_in_token(token, newChar);
                token->type = COMMENT;
            }
            else
            {
              push_char_in_token(token, newChar);
            }
        break;
        case '"':
          if (token->type != STRING_LITERAL &&
                token->type != COMMENT &&
                token->type != POSSIBLE_COMMENT_END &&
                token->type != COMMENT_LINE &&
                token->type != WHITE_SPACE)
            {
               push_token_in_queue(queue, &token);
               push_char_in_token(token, newChar);
               token->type = STRING_LITERAL;
            }
            else if (token->type == WHITE_SPACE)
            {
                token->type = STRING_LITERAL;
                push_char_in_token(token, newChar);
            }
            else if (token->type == STRING_LITERAL)
            {
                push_char_in_token(token, newChar);
                push_token_in_queue(queue, &token);
            }
            else if (token->type == POSSIBLE_COMMENT_END)
            {
                push_char_in_token(token, newChar);
                token->type = COMMENT;
            }
            else 
            {
                push_char_in_token(token, newChar);
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
