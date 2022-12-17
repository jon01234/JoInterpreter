#include "include/Token.h"

Token* CreateToken(char* value, unsigned int type)
{
    Token* token = calloc(1, sizeof(Token));

    token->value = value;
    token->type = type;

    return token;
}