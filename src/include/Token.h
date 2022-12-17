#ifndef Token_H
#define Token_H

#include <stdlib.h>

typedef struct
{
    char* value;

    enum
    {
        TOKEN_ID,
        TOKEN_STRING,
        TOKEN_LPAREN,
        TOKEN_RPAREN,
        TOKEN_ARROW,
        TOKEN_EQUALS,
        TOKEN_COMMA,
        TOKEN_LBRACKET,
        TOKEN_RBRACKET,
        TOKEN_INT,
        TOKEN_LBRACE,
        TOKEN_RBRACE,
        TOKEN_SEMI,
        TOKEN_UNKNOWN,
        TOKEN_EOF
    } type;
} Token;

/// @brief Constructor for Token struct
/// @param value 
/// @param type 
/// @return Token* 
Token* CreateToken(char* value, unsigned int type);

#endif