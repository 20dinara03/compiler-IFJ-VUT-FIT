/**
 * @file realscanner.c
 * @author xgarip00
 * @brief Real code scanner
 * @version 0.1
 */
#include <stdio.h>
#include "realscanner.h"
#include "../main.h"

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
    token_t *token = init_token();
    token->push_char(token, cur_char);
    token->type = INT_LITERAL;
    bool token_end = false;

    while (!token_end)
    {
        char current = fgetc(program.src);

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
            if (token->type == EXPONENTA)
            {
                token->push_char(token, current);
                token->type = EXPONENTAL_OPERATOR;
            }
            break;
        default:
            fseek(program.src, -1, SEEK_CUR);
            token_end = true;
        }
    }
    return (token);
}

token_t *scan_identifier(char cur_char)
{
    token_t *token = init_token();
    token->push_char(token, cur_char);
    token->type = IDENTIFIER;
    bool token_end = false;

    while (!token_end)
    {
        char current = fgetc(program.src);

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
            fseek(program.src, -1, SEEK_CUR);
            token_end = true;
        }
    }
    return (token);
}

token_t *scan_variable_identifier(char cur_char)
{
    token_t *token = init_token();
    token->push_char(token, cur_char);
    token->type = VARIABLE_IDENTIFIER;
    bool token_end = false;

    while (!token_end)
    {
        char current = fgetc(program.src);

        switch (current)
        {
        case 'A' ... 'Z':
        case 'a' ... 'z':
        case '_':
        case '0' ... '9':
            token->push_char(token, current);
            break;
        default:
            fseek(program.src, -1, SEEK_CUR);
            token_end = true;
        }
    }
    return (token);
}

token_t *scan_head(char cur_char)
{
    token_t *token = init_token();
    token->push_char(token, cur_char);
    token->type = POSSIBLE_HEAD;
    bool token_end = false;

    while (!token_end)
    {
        char current = fgetc(program.src);

        switch (current)
        {
        case '?':
            if (token->type == POSSIBLE_HEAD)
            {
                token->push_char(token, current);
                token->type = HEAD;
            }
            break;
        case 'p':
            if (token->type == HEAD)
            {
                token->push_char(token, current);
                token->type = KEYWORD_p;
            }
            else if (token->type == KEYWORD_h)
            {
                token->push_char(token, current);
                token->type = HEAD;
                token_end = true;
            }
            break;
        case 'h':
            if (token->type == KEYWORD_p)
            {
                token->push_char(token, current);
                token->type = KEYWORD_h;
            }
            break;
        case '=':
            if (token->type == POSSIBLE_HEAD)
            {
                token->push_char(token, current);
                token->type = OPERATOR_LESS_OR_EQUAL;
                token_end = true;
            }
            break;
        default:
            if (token->type == POSSIBLE_HEAD)
            {
                token->type = OPERATOR_LESS;
            }
            fseek(program.src, -1, SEEK_CUR);
            token_end = true;
        }
    }
    return (token);
}

token_t *scan_end(char cur_char)
{
    token_t *token = init_token();
    token->push_char(token, cur_char);
    token->type = POSSIBLE_END;
    bool token_end = false;

    while (!token_end)
    {
        char current = fgetc(program.src);

        switch (current)
        {
        case '>':
            if (token->type == POSSIBLE_END)
            {
                token->push_char(token, current);
                token->type = END;
                token_end = true;
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
            fseek(program.src, -1, SEEK_CUR);
            token_end = true;
        }
    }
    return (token);
}

token_t *scan_comment(char cur_char)
{
    token_t *token = init_token();
    token->push_char(token, cur_char);
    token->type = POSSIBLE_COMMENT;
    bool token_end = false;

    while (!token_end)
    {
        char current = fgetc(program.src);

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
                fseek(program.src, -1, SEEK_CUR);
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
                fseek(program.src, -1, SEEK_CUR);
                token_end = true;
            }
            break;
        default:
            if (token->type == POSSIBLE_COMMENT)
            {
                token->type = OPERATOR_DIVIDE;
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
    token_t *token = init_token();
    token->push_char(token, cur_char);
    token->type = type;
    bool token_end = false;

    while (!token_end)
    {
        char current = fgetc(program.src);

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
            fseek(program.src, -1, SEEK_CUR);
            token_end = true;
        }
    }
    return (token);
}

token_t *scan_string(char cur_char)
{
    token_t *token = init_token();
    token->push_char(token, cur_char);
    token->type = STRING_LITERAL;
    bool token_end = false;

    while (!token_end)
    {
        char current = fgetc(program.src);

        switch (current)
        {
        case '"':
            token->push_char(token, current);
            token_end = true;
            break;
        default:
            token->push_char(token, current);
        }
    }
    return (token);
}

token_t *scan_slash(char cur_char)
{
    token_t *token = init_token();
    token->push_char(token, cur_char);
    token->type = OPERATOR_SLASH;
    bool token_end = false;

    while (!token_end)
    {
        char current = fgetc(program.src);

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
            fseek(program.src, -1, SEEK_CUR);
            token_end = true;
        }
    }
    return (token);
}

token_t *Scan()
{
    /*Reading 1 character from a file until we reach the end of it*/
    char current_char;
    while (!feof(program.src))
    {
        current_char = fgetc(program.src);

        switch (current_char)
        {
        case '*':
            program.scanner->current_token = scan_operator(current_char, OPERATOR_MULTIPLY);
            break;
        case '/':
            program.scanner->current_token = scan_comment(current_char);
            break;
        case '"':
            program.scanner->current_token = scan_string(current_char);
            break;
        case '+':
            program.scanner->current_token = scan_operator(current_char, OPERATOR_PLUS);
            break;
        case '-':
            program.scanner->current_token = scan_operator(current_char, OPERATOR_MINUS);
            break;
        case '(':
            program.scanner->current_token = scan_operator(current_char, OPERATOR_LEFT_BRACKET);
            break;
        case ')':
            program.scanner->current_token = scan_operator(current_char, OPERATOR_RIGHT_BRACKET);
            break;
        case ':':
            program.scanner->current_token = scan_operator(current_char, OPERATOR_COLON);
            break;

        case ',':
            program.scanner->current_token = scan_operator(current_char, OPERATOR_COMMA);
            break;

        case '{':
            program.scanner->current_token = scan_operator(current_char, OPERATOR_LEFT_CURLY_BRACKET);
            break;

        case '}':
            program.scanner->current_token = scan_operator(current_char, OPERATOR_RIGHT_CURLY_BRACKET);
            break;
        case ';':
            program.scanner->current_token = scan_operator(current_char, OPERATOR_SEMICOLON);
            break;

        case '\\':
            program.scanner->current_token = scan_slash(current_char);
            break;

        case '.':
            program.scanner->current_token = scan_operator(current_char, OPERATOR_POINT);
            break;

        case '0' ... '9':

            program.scanner->current_token = scan_number(current_char);
            break;

        case '<':
            program.scanner->current_token = scan_head(current_char);
            break;

        case '>':
            program.scanner->current_token = scan_operator(current_char, OPERATOR_GREATER);
            break;

        case '?':
            program.scanner->current_token = scan_end(current_char);
            break;

        case '=':
            program.scanner->current_token = scan_operator(current_char, OPERATOR_EQUAL);
            break;

        case '!':
            program.scanner->current_token = scan_operator(current_char, OPERATOR_NOT);
            break;

        case '|':
            program.scanner->current_token = scan_operator(current_char, POSSIBLE_OR);
            break;
        case '&':
            program.scanner->current_token = scan_operator(current_char, POSSIBLE_AND);
            break;
        case 'A' ... 'Z':
        case 'a' ... 'z':
            program.scanner->current_token = scan_identifier(current_char);
            break;
        case '$':
            program.scanner->current_token = scan_variable_identifier(current_char);
            break;
        case '_':
            program.scanner->current_token = scan_identifier(current_char);
            break;
        default:
            continue;
        }

        char* string = malloc(sizeof(char) * 2048);
        if(0 <= sprintf(string, "( %s : %s )\n", program.scanner->current_token->decode(program.scanner->current_token->type),
            program.scanner->current_token->text)) {
            program.scanner->logger->debug(program.scanner->logger, string);
            if (string != NULL) free(string);
        }
        return program.scanner->current_token;
    }
    program.scanner->current_token = NULL;
    return NULL;
}

scanner_t *init_scanner()
{
    scanner_t *scanner = NULL;
    memo_allocate(scanner, scanner_t, 1);

    scanner->get_next_token = Scan;
    scanner->free = destruct_scanner_t;
    scanner->logger = get_logging(scanner);

    return scanner;
}

void destruct_scanner_t(scanner_t *scanner)
{
    if (scanner != NULL)
    {
        if (scanner->logger != NULL) free_scanner_logging(scanner->logger);
        free(scanner);
    }
}
