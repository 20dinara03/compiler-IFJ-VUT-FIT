/**
 * @file realscanner.c
 * @author xgarip00
 * @brief Real code scanner
 * @version 0.1
 */
#include "realscanner.h"
#include "input_stack.h"
#include "../main.h"

token_t* scan_number(char cur_char)
{
    token_t* token = init_token();
    token->push_char(token, cur_char);
    token->type = INT_LITERAL;
    bool token_end = false;

    while (!token_end)
    {
        char current = fgetc(program.src);

        switch(current)
        {
          case '0'...'9':
          token->push_char(token, current);
          if (token->type == EXPONENTA || token->type == EXPONENTAL_OPERATOR)
          {
            token->type = DOUBLE_LITERAL;
          }
          break;
          case '.':
          if (token->type == INT_LITERAL)
          {
            token->push_char(token, current);
            token->type = DOUBLE_LITERAL;
          }
          break;
          case 'E':
          case 'e':
          if (token->type == DOUBLE_LITERAL)
          {
            token->push_char(token, current);
            token->type = EXPONENTA;
          }
          break;
          case '+':
          case '-':
          if(token->type == EXPONENTA)
          {
            token->push_char(token, current);
            token->type = EXPONENTAL_OPERATOR;
          }
          break;
          default:
          token_end = true;
        }
    }
    return(token);
}

token_t* scan_identifier(char cur_char)
{
    token_t* token = init_token();
    token->push_char(token, cur_char);
    token->type = IDENTIFIER;
    bool token_end = false;

    while (!token_end)
    {
        char current = fgetc(program.src);

        switch(current)
        {
            case 'A'... 'Z':
            case 'a'...'z':
            case '_':
            case '0'...'9':
             token->push_char(token, current);
            break;
            default:
            token_end = true;
        }
    }
  return(token);
}

token_t* scan_variable_identifier(char cur_char)
{
    token_t* token = init_token();
    token->push_char(token, cur_char);
    token->type = VARIABLE_IDENTIFIER;
    bool token_end = false;

    while (!token_end)
    {
        char current = fgetc(program.src);

        switch(current)
        {
            case 'A'... 'Z':
            case 'a'...'z':
            case '_':
            case '0'...'9':
             token->push_char(token, current);
            break;
            default:
            token_end = true;
        }
    }
  return(token);
}

token_t* scan_head(char cur_char)
{
    token_t* token = init_token();
    token->push_char(token, cur_char);
    token->type = POSSIBLE_HEAD;
    bool token_end = false;

    while (!token_end)
    {
        char current = fgetc(program.src);

        switch(current)
        {
            case '?':
            if (token->type == POSSIBLE_HEAD)
            {
            token->push_char(token, current);
            token->type =HEAD;
            }
            break;
            case 'p':
            if (token->type == HEAD)
            {
             token->push_char(token, current);
             token->type =KEYWORD_p;
            }
            else if (token->type == KEYWORD_h)
            {
             token->push_char(token, current);
             token->type =HEAD;
             token_end = true;
            }
            break;
            case 'h':
            if (token->type == KEYWORD_p)
            {
             token->push_char(token, current);
             token->type =KEYWORD_h;
            }
            break;
            case '=':
            if (token->type == POSSIBLE_HEAD)
            {
             token->push_char(token, current);
             token->type =OPERATOR_LESS_OR_EQUAL;
             token_end = true;
            }
            default:
            if (token->type == POSSIBLE_HEAD)
            {
                token->type = OPERATOR_LESS;
            }
            token_end = true;
        }
    }
  return(token);
}

token_t* scan_end(char cur_char)
{
    token_t* token = init_token();
    token->push_char(token, cur_char);
    token->type = POSSIBLE_END;
    bool token_end = false;

    while (!token_end)
    {
        char current = fgetc(program.src);

        switch(current)
        {
            case '>':
            if (token->type == POSSIBLE_END)
            {
             token->push_char(token, current);
             token->type = END;
             token_end = true;
            }
            break;
            case 'A'...'Z':
            case 'a'...'z':
            token->push_char(token, current);
            token->type = TYPE_IDENTIFIER;
            default:
            token_end = true;
        }
    }
  return(token);
}

token_t* scan_comment(char cur_char)
{
    token_t* token = init_token();
    token->push_char(token, cur_char);
    token->type = POSSIBLE_COMMENT;
    bool token_end = false;

    while (!token_end)
    {
        char current = fgetc(program.src);

        switch(current)
        {
            case '/':
            if (token->type == POSSIBLE_COMMENT)
            {
            token->push_char(token, current);
            token->type = COMMENT_LINE;
            }
            else if (token->type == POSSIBLE_COMMENT_END)
            {
                token->push_char(token, current);
                token->type = COMMENT;
                token_end = true;
            }
            break;
            case '*':
            if (token->type == POSSIBLE_COMMENT)
            {
             token->push_char(token, current);
             token->type = COMMENT;
            }
            else if (token->type == COMMENT)
            {
             token->push_char(token, current);
             token->type = POSSIBLE_COMMENT_END; 
            }
            break;
            case '\n':
            case '\r':
            if (token->type == COMMENT_LINE)
            {
                token_end = true;
            }
            break;
            default:
            if (token->type == POSSIBLE_COMMENT)
            {
                token->type=OPERATOR_DIVIDE;
                token_end = true;
            }
            else if (token->type == POSSIBLE_COMMENT_END)
            {
                token->push_char(token, current);
                token->type = COMMENT;
            }
            else 
            {
                token->push_char(token, current);
            }
        }
    }
  return(token);
}

token_t* scan_operator(char cur_char,types_t type)
{
    token_t* token = init_token();
    token->push_char(token, cur_char);
    token->type = type;
    bool token_end = false;

    while (!token_end)
    {
        char current = fgetc(program.src);

        switch(current)
        {
            case'=':
            if (token->type == OPERATOR_NOT)
            {
             token->push_char(token, current);
             token->type = OPERATOR_NOT_EQUAL;
            }
            else if (token->type = OPERATOR_NOT_EQUAL)
            {
                token->push_char(token, current);
                token_end = true;
            }
            else if (token->type == OPERATOR_GREATER)
            {
                token->push_char(token, current);
                token->type = OPERATOR_GREATER_OR_EQUAL;
                token_end = true;
            }
            else if (token->type == OPERATOR_EQUAL)
            {
                token->push_char(token, current);
            }
            break;
            case '|':
            if (token->type == POSSIBLE_OR)
            {
                token->push_char(token, current);
                token->type = OPERATOR_OR;
                token_end = true;
            }
            break;
            case '&':
            if (token->type == POSSIBLE_AND)
            {
                token->push_char(token, current);
                token->type = OPERATOR_AND;
                token_end = true;
            }
            break;
            default:
            token_end = true;
        }
    }
  return(token);
}

token_t* scan_string(char cur_char)
{
    token_t* token = init_token();
    token->push_char(token, cur_char);
    token->type = STRING_LITERAL;
    bool token_end = false;

    while (!token_end)
    {
        char current = fgetc(program.src);

        switch(current)
        {
            case '"':
             token->push_char(token, current);
             token_end = true;
            break;
            default:
            token->push_char(token, current);
        }
    }
  return(token);
}

token_t* scan_slash(char cur_char)
{
    token_t* token = init_token();
    token->push_char(token, cur_char);
    token->type = OPERATOR_SLASH;
    bool token_end = false;

    while (!token_end)
    {
        char current = fgetc(program.src);

        switch(current)
        {
            case 'n':
            case 'r':
            case 't':
            case '\\':
            case '$':
            case '"':
            if (token->type == OPERATOR_SLASH)
            {
            token->push_char(token, current);
            token->type = ESCAPE_SEQUENCE;
            token_end = true;
            }
            break;
            case 'x':
            if (token->type == OPERATOR_SLASH)
            {
                token->push_char(token, current);
                token->type = HEXADECIMAL_NUMBER_X;
            }
            case 'A'... 'F':
            case 'a'...'f':
            case '8'...'9':
             if (token->type == HEXADECIMAL_NUMBER_X)
             {
              token->push_char(token, current);
              token->type = HEXADECIMAL_NUMBER_1;
             }
             else if (token->type == HEXADECIMAL_NUMBER_1)
             {
              token->push_char(token, current);
              token->type = HEXADECIMAL_NUMBER;
              token_end = true;
             }
            break;
            case '0'...'7':
            if (token->type == HEXADECIMAL_NUMBER_X)
             {
              token->push_char(token, current);
              token->type = HEXADECIMAL_NUMBER_1;
             }
             else if (token->type == HEXADECIMAL_NUMBER_1)
             {
              token->push_char(token, current);
              token->type = HEXADECIMAL_NUMBER;
              token_end = true;
             }
             else if (token->type == OPERATOR_SLASH)
             {
                token->push_char(token, current);
                token->type = OCTAVE_NUMBER_1;
             }
             else if (token->type == OCTAVE_NUMBER_1)
             {
                token->push_char(token, current);
                token->type = OCTAVE_NUMBER_2;
             }
             else if (token->type == OCTAVE_NUMBER_2)
             {
                token->push_char(token, current);
                token->type = OCTAVE_NUMBER;
                token_end = true;
             }
            default:
            token_end = true;
        }
    }
  return(token);
}

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
        case '*':
         token->type = OPERATOR_MULTIPLY;
         scan_operator(current_char, token->type);
        break;

        case '/':
         scan_comment(current_char);
        break;

        case '"':
         scan_string(current_char);
        break;

        case '+':
         token->type = OPERATOR_PLUS;
         scan_operator(current_char, token->type);
        break;

        case '-':
         token->type = OPERATOR_MINUS;
         scan_operator(current_char, token->type);
        break;

        case '(':
         token->type = OPERATOR_LEFT_BRACKET;
         scan_operator(current_char, token->type);
        break;

        case ')':
         token->type = OPERATOR_RIGHT_BRACKET;
         scan_operator(current_char, token->type);
        break;
        
        case ':':
         token->type = OPERATOR_COLON;
         scan_operator(current_char, token->type);
        break;

        case ',':
         token->type = OPERATOR_COMMA;
         scan_operator(current_char, token->type);
        break;

        case '{':
         token->type = OPERATOR_LEFT_CURLY_BRACKET;
         scan_operator(current_char, token->type);
        break;

        case '}':
         token->type = OPERATOR_RIGHT_CURLY_BRACKET;
         scan_operator(current_char, token->type);
        break;

        case ';':
         token->type = OPERATOR_SEMICOLON;
         scan_operator(current_char, token->type);
        break;

        case '\\':
         scan_slash(current_char);
        break;

        case '.':
         token->type = OPERATOR_POINT;
         scan_operator(current_char, token->type);
        break;

        case '0' ... '9':
         scan_number(current_char);
        break;

        case '<':
         scan_head(current_char);
        break;

        case '>':
         token->type = OPERATOR_GREATER;
         scan_operator(current_char, token->type);
        break;

        case '?':
         scan_end(current_char);
        break;

        case '=':
         token->type = OPERATOR_EQUAL;
         scan_operator(current_char, token->type);
        break;

        case '!':
         token->type = OPERATOR_NOT;
         scan_operator(current_char, token->type);
        break;

        case '|':
         token->type = POSSIBLE_OR;
         scan_operator(current_char, token->type);
        break;

        case '&':
         token->type = POSSIBLE_AND;
         scan_operator(current_char, token->type);
        break;

        case 'A'...'Z':
        case 'a'...'z':
          scan_identifier(current_char);
        break;

        case '$': 
          scan_variable_identifier(current_char);
        break;

        case '_':
          scan_identifier(current_char);
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
