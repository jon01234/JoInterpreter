#ifndef Lexer_H
#define Lexer_H

#include "Token.h"
#include "Print.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/** @brief Lexer struct to keep the lexer together */
typedef struct Lexer
{
    char* code;
    size_t code_size;
    char current_char;

    // Current index of the current character in relation to the source code 
    unsigned int i;
    unsigned int lineno;
} Lexer;

/** @brief Constructor for the lexer
 * @param code
 * @return Lexer*
*/
Lexer* CreateLexer(char* code);

/** @brief Advances the lexer passed in to the next character
 * @param lexer
*/
void Lexer_Advance(Lexer* lexer);
/** @brief Advances through all the whitespace if there is any
 * @param lexer
*/
void Lexer_SkipWhitespace(Lexer* lexer);
/** @brief Gets the identifier in current position of current char
 * @param lexer
 * @return void
*/
Token* Lexer_GetIdentifier(Lexer* lexer);
/** @brief Gets the string in current position of current char
 * @param lexer
 * @return The current char
*/
Token* Lexer_GetString(Lexer* lexer);
/** @brief Gets the number in current position of current char
 * @param lexer
 * @return The current number
*/
Token* Lexer_GetNumber(Lexer* lexer);

/** Gets the token in current position of current char
 * This calls the functions:
 *     Advance
 *     SkipWhitespace
 *     GetIdentifier
 *     GetString
 *     GetNumber
 *
 * at the right time
 * @param lexer
 * @return The next token
*/
Token* Lexer_GetNextToken(Lexer* lexer);

char Lexer_Peak(Lexer* lexer, int n);
char Lexer_PeakNext(Lexer* lexer);

#endif
