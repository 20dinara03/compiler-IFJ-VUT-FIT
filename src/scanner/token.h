#ifndef TOKEN
#define TOKEN

#include "../main.h"

/* token types constants */
typedef enum
{
    KEYWORD,
    VARIABLE_IDENTIFIER,
    IDENTIFIER,
    STRING_LITERAL,
    INT_LITERAL,
    DOUBLE_LITERAL,
    WHITE_SPACE,
    COMMENT,
    POSSIBLE_HEAD,
    POSSIBLE_OR,
    POSSIBLE_AND,
    POSSIBLE_COMMENT,
    POSSIBLE_COMMENT_END,
    COMMENT_LINE,
    KEYWORD_h,
    KEYWORD_p,
    EXPONENTA,
    OPERATOR_PLUS,
    OPERATOR_MINUS,
    OPERATOR_MULTIPLY,
    OPERATOR_DIVIDE,
    OPERATOR_LEFT_BRACKET,
    OPERATOR_RIGHT_BRACKET,
    OPERATOR_POINT,
    OPERATOR_GREATER,
    OPERATOR_LESS,
    OPERATOR_EQUAL,
    OPERATOR_LESS_OR_EQUAL,
    OPERATOR_GREATER_OR_EQUAL,
    OPERATOR_NOT,
    OPERATOR_NOT_EQUAL,
    OPERATOR_OR,
    OPERATOR_AND,
    OPERATOR_SEMICOLON,
    OPERATOR_COLON,
    OPERATOR_COMMA,
    OPERATOR_LEFT_CURLY_BRACKET,
    OPERATOR_RIGHT_CURLY_BRACKET,
    //extra types
    VOID_LITERAL,
    NONE,
    EXPONENTAL_OPERATOR,
    OPERATOR_SLASH,
    HEAD,
    END,
    POSSIBLE_END,
    TYPE_IDENTIFIER,
    HEXADECIMAL_NUMBER_X,
    ESCAPE_SEQUENCE,
    HEXADECIMAL_NUMBER_1,
    HEXADECIMAL_NUMBER,
    OCTAVE_NUMBER,
    OCTAVE_NUMBER_1,
    OCTAVE_NUMBER_2,
    KEYWORD_IF,
    KEYWORD_ELSE,
    KEYWORD_FLOAT,
    KEYWORD_FUNCTION,
    KEYWORD_INT,
    KEYWORD_NULL,
    KEYWORD_RETURN,
    KEYWORD_VOID,
    KEYWORD_WHILE,
    KEYWORD_STRING
} types_t;


typedef struct token_t token_t;

/* token struct */
struct token_t
{
    string text;
    types_t type;
    const char* (*decode)(types_t type);
    size_t offsetX;
    size_t offsetY;
    /**
     * @brief Appends symbol at the end of the token text
     *
     * @param token pointer to a target token_t
     * @param ch input char
     */
    void (*push_char)(token_t *, char);
    /**
     *
     * @brief Resets token (creates new one)
     *
     * @param token pointer to token_t
     */
    void (*reset)(token_t **);
    /**
     *
     * @brief Prints token in console, in (type : value) format
     *
     * @param token pointer to token_t
     * @param buffer string buffer
     */
    void (*debug)(token_t *);
    /**
     * @brief Destructor for token
     *
     * @param token pointer to token_t
     */
    void (*free)(token_t **);
};

/**
 * @brief Constructor for token_t
 *
 * @return new pointer to token_t
 */
token_t *init_token();

#endif /* TOKEN */
