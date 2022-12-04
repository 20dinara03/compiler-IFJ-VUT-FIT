/**
 * @file realscanner.c
 * @author xgarip00
 * @brief Real code scanner
 * @version 0.1
 */
#include <stdio.h>
#include "realscanner.h"
#include "../main.h"

#define init_and_push(cur_char)        \
    token_t *token = init_token();     \
    token->push_char(token, cur_char); \
    bool token_end = false;            \
    token_end = token_end;
#define fseek_and_token_end(current)   \
    if (current != EOF){ \
     fseek(stdin, -1, SEEK_CUR); \
     token_end = true; \
    } \
    else{ \
     token_end = true; \
    }

define_logging(scanner)

    void check_identifier(token_t *token)
{
    if (strcmp(token->text, "if") == 0)
    {
        token->type = KEYWORD_IF;
    }
    else if (strcmp(token->text, "else") == 0)
    {
        token->type = KEYWORD_ELSE;
    }
    else if (strcmp(token->text, "function") == 0)
    {
        token->type = KEYWORD_FUNCTION;
    }
    else if (strcmp(token->text, "float") == 0 || strcmp(token->text, "?float") == 0)
    {
        token->type = KEYWORD_FLOAT;
    }
    else if (strcmp(token->text, "string") == 0 || strcmp(token->text, "?string") == 0)
    {
        token->type = KEYWORD_STRING;
    }
    else if (strcmp(token->text, "int") == 0 || strcmp(token->text, "?int") == 0)
    {
        token->type = KEYWORD_INT;
    }
    else if (strcmp(token->text, "null") == 0)
    {
        token->type = KEYWORD_NULL;
    }
    else if (strcmp(token->text, "return") == 0)
    {
        token->type = KEYWORD_RETURN;
    }
    else if (strcmp(token->text, "void") == 0)
    {
        token->type = KEYWORD_VOID;
    }
    else if (strcmp(token->text, "while") == 0)
    {
        token->type = KEYWORD_WHILE;
    }
}

token_t *scan_number(char cur_char)
{
    init_and_push(cur_char);
    token->type = INT_LITERAL;
    while (!token_end)
    {
        char current = (char)fgetc(stdin);
        switch (current)
        {
        case '0' ... '9':
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
            else
            {
                fseek_and_token_end(current)
            }
            break;
        case 'E':
        case 'e':
            if (token->type == DOUBLE_LITERAL)
            {
                token->push_char(token, current);
                token->type = EXPONENTA;
            }
            else
            {
                fseek_and_token_end(current);
            }
            break;
        case '+':
        case '-':
            if (token->type == EXPONENTA)
            {
                token->push_char(token, current);
                token->type = EXPONENTAL_OPERATOR;
            }
            else
            {
                fseek_and_token_end(current)
            }
            break;
        default:
         if (current != EOF){
            fseek_and_token_end(current)
         }
         else 
         {
            token_end = true;
         }
        }
    }
    return (token);
}

token_t *scan_identifier(char cur_char)
{
    init_and_push(cur_char);
    token->type = IDENTIFIER;
    while (!token_end)
    {
        char current = (char)fgetc(stdin);
        
        switch (current)
        {
        case 'A' ... 'Z':
        case 'a' ... 'z':
        case '_':
        case '0' ... '9':
            token->push_char(token, current);
            break;
        default:
            check_identifier(token);
            fseek_and_token_end(current);
        }
    }
    return (token);
}

token_t *scan_variable_identifier(char cur_char)
{
    init_and_push(cur_char);
    token->type = VARIABLE_IDENTIFIER;
    while (!token_end)
    {
        char current = (char)fgetc(stdin);
        
        switch (current)
        {
        case 'A' ... 'Z':
        case 'a' ... 'z':
        case '_':
        case '0' ... '9':
            token->push_char(token, current);
            break;
        default:
            fseek_and_token_end(current);
        }
    }
    return (token);
}

token_t *scan_head(char cur_char, scanner_t *self)
{
    init_and_push(cur_char);
    token->type = POSSIBLE_HEAD;
    while (!token_end)
    {
        char current = (char)fgetc(stdin);
        
        switch (current)
        {
        case '?':
            if (token->type == POSSIBLE_HEAD)
            {
                token->push_char(token, current);
                token->type = POSSIBLE_HEAD_2;
            }
            else
            {
                fseek_and_token_end(current);
            }
            break;
        case 'p':
            if (token->type == POSSIBLE_HEAD_2)
            {
                token->push_char(token, current);
                token->type = KEYWORD_p;
            }
            else if (token->type == KEYWORD_h)
            {
                token->push_char(token, current);
                token->type = HEAD;
                token_end = true;
                self->white_space_reading = false;
            }
            else
            {
                fseek_and_token_end(current);
            }
            break;
        case 'h':
            if (token->type == KEYWORD_p)
            {
                token->push_char(token, current);
                token->type = KEYWORD_h;
            }
            else
            {
                fseek_and_token_end(current);
            }
            break;
        case '=':
            if (token->type == POSSIBLE_HEAD)
            {
                token->push_char(token, current);
                token->type = OPERATOR_LESS_OR_EQUAL;
                token_end = true;
            }
            else
            {
                fseek_and_token_end(current);
            }
            break;
        default:
            if (token->type == POSSIBLE_HEAD)
            {
                token->type = OPERATOR_LESS;
            }
            fseek_and_token_end(current);
        }
    }
    return (token);
}

token_t *scan_end(char cur_char, scanner_t *self)
{
    init_and_push(cur_char);
    token->type = POSSIBLE_END;
    while (!token_end)
    {
        char current = (char)fgetc(stdin);
        
        switch (current)
        {
        case '>':
            if (token->type == POSSIBLE_END)
            {
                token->push_char(token, current);
                token->type = END;
                token_end = true;
                self->white_space_reading = true;
            }
            else
            {
                fseek_and_token_end(current);
            }
            break;
        case 'A' ... 'Z':
        case 'a' ... 'z':
            token->push_char(token, current);
            token->type = TYPE_IDENTIFIER;
            break;
        default:
            if (token->type == TYPE_IDENTIFIER)
            {
                check_identifier(token);
            }
            fseek_and_token_end(current);
        }
    }
    return (token);
}

token_t *scan_comment(char cur_char)
{
    init_and_push(cur_char);
    token->type = POSSIBLE_COMMENT;
    while (!token_end)
    {
        char current = (char)fgetc(stdin);
        
        switch (current)
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
            else if (token->type == COMMENT)
            {
                token->push_char(token, current);
            }
            else if (token->type == COMMENT_LINE)
            {
                token->push_char(token, current);
            }
            else
            {
                fseek_and_token_end(current);
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
            else if (token->type ==POSSIBLE_COMMENT_END)
            {
                token->push_char(token, current);
                token->type = POSSIBLE_COMMENT_END;
            }
            else if (token->type == COMMENT_LINE)
            {
                token->push_char(token, current);
            }
            else
            {
                fseek_and_token_end(current);
            }
            break;
        case '\n':
        case '\r':
            if (token->type == COMMENT_LINE)
            {
                fseek(stdin, -1, SEEK_CUR);
                token_end = true;
            }
            break;
        default:
            if (token->type == POSSIBLE_COMMENT)
            {
                token->type = OPERATOR_DIVIDE;
                fseek(stdin, -1, SEEK_CUR);
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
    return (token);
}

token_t *scan_operator(char cur_char, types_t type)
{
    init_and_push(cur_char);
    token->type = type;
    while (!token_end)
    {
        char current = (char)fgetc(stdin);
        
        switch (current)
        {
        case '=':
            if (token->type == OPERATOR_NOT)
            {
                token->push_char(token, current);
                token->type = OPERATOR_NOT_EQUAL;
            }
            else if (token->type == OPERATOR_NOT_EQUAL)
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
            fseek_and_token_end(current);
        }
    }
    return (token);
}

token_t *scan_string(char cur_char)
{
    init_and_push(cur_char);
    token->type = STRING_LITERAL;
    bool slash = false;
    while (!token_end)
    {
        char current = (char)fgetc(stdin);
        switch(current){
        case '\"':
            if (slash)
            {
             token->push_char(token, '\\');
             token->push_char(token, '\"');
             slash = false;
            }
            else
            {
             token->push_char(token, current); 
             token_end = true;
            }
        break;
        case ' ':
            if (slash)
            {
             token->push_char(token, '\\');
             token->push_char(token, '0');
             token->push_char(token, '9');
             token->push_char(token, '2');
             slash = false;
            }
            token->push_char(token, '\\');
            token->push_char(token, '0');
            token->push_char(token, '3');
            token->push_char(token, '2');
        break;
        case '\\':
            if (slash)
            {
              token->push_char(token, '\\');
             token->push_char(token, '0');
             token->push_char(token, '9');
             token->push_char(token, '2');
             slash = false;
            }
            else{
            slash = true;
            }
        break;
        case 'n':
            if (slash)
            {
             token->push_char(token, '\\');
             token->push_char(token, '0');
             token->push_char(token, '1');
             token->push_char(token, '0');
             slash = false;
            }
            else
            {
                token->push_char(token, current);
            }
        break;
        case '#':
           if (slash)
            {
             token->push_char(token, '\\');
             token->push_char(token, '0');
             token->push_char(token, '9');
             token->push_char(token, '2');
             slash = false;
            }
            token->push_char(token, '\\');
            token->push_char(token, '0');
            token->push_char(token, '3');
            token->push_char(token, '5');
        break;
        case '0'...'7':
            if (slash)
            {
             token->push_char(token, '\\');
             token->push_char(token, current);
             slash = false;
            }
            else
            {
                token->push_char(token, current);
            }
        break;
        default:
            if (slash)
            {
             token->push_char(token, '\\');
             token->push_char(token, '0');
             token->push_char(token, '9');
             token->push_char(token, '2');
             slash = false; 
            }
            token->push_char(token, current);
       }
    }
    return (token);
}

token_t *scan_slash(char cur_char)
{
    init_and_push(cur_char);
    token->type = OPERATOR_SLASH;
    while (!token_end)
    {
        char current = (char)fgetc(stdin);
        switch (current)
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
            break;
        case 'A' ... 'F':
        case 'a' ... 'f':
        case '8' ... '9':
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
        case '0' ... '7':
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
            break;
        default:
            fseek_and_token_end(current);
        }
    }
    return (token);
}

token_t *Scan(scanner_t *self)
{
    if (self->current_token != NULL){
    self->current_token->free(&self->current_token);
    }
    while (!feof(stdin))
    {
        char current_char;
        current_char = (char)fgetc(stdin);
        switch (current_char)
        {
        case '*':
            self->current_token = scan_operator(current_char, OPERATOR_MULTIPLY);
            break;
        case '/':
            self->current_token = scan_comment(current_char);
            break;
        case '"':
            self->current_token = scan_string(current_char);
            break;
        case '+':
            self->current_token = scan_operator(current_char, OPERATOR_PLUS);
            break;
        case '-':
            self->current_token = scan_operator(current_char, OPERATOR_MINUS);
            break;
        case '(':
            self->current_token = scan_operator(current_char, OPERATOR_LEFT_BRACKET);
            break;
        case ')':
            self->current_token = scan_operator(current_char, OPERATOR_RIGHT_BRACKET);
            break;
        case ':':
            self->current_token = scan_operator(current_char, OPERATOR_COLON);
            break;
        case ',':
            self->current_token = scan_operator(current_char, OPERATOR_COMMA);
            break;
        case '{':
            self->current_token = scan_operator(current_char, OPERATOR_LEFT_CURLY_BRACKET);
            break;
        case '}':
            self->current_token = scan_operator(current_char, OPERATOR_RIGHT_CURLY_BRACKET);
            break;
        case ';':
            self->current_token = scan_operator(current_char, OPERATOR_SEMICOLON);
            break;
        case '\\':
            self->current_token = scan_slash(current_char);
            break;
        case '.':
            self->current_token = scan_operator(current_char, OPERATOR_POINT);
            break;
        case '0' ... '9':
            self->current_token = scan_number(current_char);
            break;
        case '<':
            self->current_token = scan_head(current_char, self);
            break;
        case '>':
            self->current_token = scan_operator(current_char, OPERATOR_GREATER);
            break;
        case '?':
            self->current_token = scan_end(current_char, self);
            break;
        case '=':
            self->current_token = scan_operator(current_char, OPERATOR_EQUAL);
            break;
        case '!':
            self->current_token = scan_operator(current_char, OPERATOR_NOT);
            break;
        case '|':
            self->current_token = scan_operator(current_char, POSSIBLE_OR);
            break;
        case '&':
            self->current_token = scan_operator(current_char, POSSIBLE_AND);
            break;
        case 'A' ... 'Z':
        case 'a' ... 'z':
            self->current_token = scan_identifier(current_char);
            break;
        case '$':
            self->current_token = scan_variable_identifier(current_char);
            break;
        case '_':
            self->current_token = scan_identifier(current_char);
            break;
        case ' ':
        case '\t':
            if (self->white_space_reading)
            {
                self->current_token = init_token();
                self->current_token->push_char(self->current_token, current_char);
            }
            else
                continue;
            break;
        default:
            continue;
        }

        char *string = malloc(sizeof(char) * 2048);
        if (0 <= sprintf(string, "( %s : %s )\n", self->current_token->decode(self->current_token->type),
                         self->current_token->text))
        {
            self->logger->debug(self->logger, string); //@SEGFAULT
            if (string != NULL)
                free(string);
        }
        return self->current_token;
    }
 self->current_token = NULL;
 return self->current_token;
}
scanner_t *init_scanner()
{
    scanner_t *self = NULL;

    memo_allocate(self, scanner_t, 1);

    self->get_next_token = Scan;
    self->white_space_reading = true;
    self->free = destruct_scanner_t;
    self->logger = get_logging(scanner);
    return self;
}
void destruct_scanner_t(scanner_t *self)
{
    if (self != NULL)
    {
        if (self->logger != NULL)
         free_scanner_logging(self->logger);
        free(self);
    }
}

