#ifndef Parser_H
#define Parser_H

#include "Lexer.h"
#include "Token.h"
#include "Node.h"
#include "Print.h"
#include <string.h>
#include <stdlib.h>

/**
 * @brief Struct that stores information used for the parser
 *
 */
typedef struct Parser
{
    Lexer* lexer;
    Token* current_token;
    Token* prev_token;
} Parser;

/**
 * @brief Constructor for parser
 *
 * @param lexer
 * @return
 */
Parser* CreateParser(Lexer* lexer);

// Methods that are used in parser
void Parser_Next(Parser* parser);
void Parser_Expect(Parser* parser, int token);

/**
 * @brief This is the function that calls the other parsing functions
 *
 *
 * @param parser
 * @return
 */
Node* Parser_Parse(Parser* parser);
Node* Parser_ParseStatements(Parser* parser);
Node* Parser_ParseStatement(Parser* parser);
Node* Parser_ParseFunction(Parser* parser);
Node* Parser_ParseFunctionBody(Parser* parser, Node* node);
Node* Parser_ParseFunctionCall(Parser* parser);
Node* Parser_ParseVariableDecl(Parser* parser);

#endif