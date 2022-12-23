#include "include/Parser.h"
#include "include/Print.h"

Parser* CreateParser(Lexer* lexer)
{
    Parser* parser = calloc(1, sizeof(Parser));
    parser->lexer = lexer;
    parser->current_token = Lexer_GetNextToken(parser->lexer); // Get the next token (Which is the first in this case)
    parser->prev_token = parser->current_token; // In the begining prev and current are the same 

    return parser;
}

void Parser_Next(Parser* parser)
{
    parser->prev_token = parser->current_token;
    parser->current_token = Lexer_GetNextToken(parser->lexer); // Get the next token
}

void Parser_Expect(Parser* parser, int token)
{
    // Continue if parser got the expected token
    if (parser->current_token->type == token)
    {
        Parser_Next(parser);
    }
    else
    {
        Error(
            "Unexpected token '%s' at line '%d'. Expected token of type '%d'",
            parser->current_token->value,
            parser->lexer->lineno,
            token
        );
    }
}

Node* Parser_Parse(Parser* parser)
{
    // Right now everything is a statement in the language
    return Parser_ParseStatements(parser);
}

Node* Parser_ParseStatements(Parser* parser)
{
    Node* node = CreateNode(NODE_PROGRAM);

    node->program = calloc(0, sizeof(Node));

    for (int i = 0; parser->current_token->type != TOKEN_EOF; i++) // Run until end of file
    {
        switch (parser->current_token->type)
        {
        case TOKEN_ID: // A statement can only start with token
            node->program = realloc(node->program, i + 1); // increase program
            node->program[i] = Parser_ParseStatement(parser);

        default: break;
        }

        node->program_size += 1; // Increase program size
    }

    return node;
}

Node* Parser_ParseStatement(Parser* parser)
{
    // We have a identifier in the current token
    if (strcmp(parser->current_token->value, "function") == 0) // function is a keyword
        return Parser_ParseFunction(parser);

    Parser_Next(parser);

    if (parser->current_token->type == TOKEN_LPAREN)
        return Parser_ParseFunctionCall(parser);
    if (parser->current_token->type == TOKEN_ARROW)
        return Parser_ParseVariableDecl(parser);

    Parser_Expect(parser, TOKEN_LPAREN);
    return CreateNode(NODE_NOOP);
}

Node* Parser_ParseFunction(Parser* parser)
{
    Node* node = CreateNode(NODE_FUNCTION_DECL);

    Parser_Expect(parser, TOKEN_ID); // Function decl ("function" keyword)
    Parser_Expect(parser, TOKEN_ID); // Function name
    node->function_name = parser->prev_token->value;
    Parser_Expect(parser, TOKEN_LPAREN); // (

    if (parser->current_token->type == TOKEN_RPAREN) // We have no args
    {
        Parser_Expect(parser, TOKEN_RPAREN); // )
        Parser_Expect(parser, TOKEN_ARROW); // ->
        Parser_Expect(parser, TOKEN_ID); // Function return type as identifier
        node->function_return_type = parser->prev_token->value;
        return Parser_ParseFunctionBody(parser, node); // Now parse the body
    }
    // Parse the arguments
    node->function_args = calloc(1, sizeof(struct Node*));
    node->function_args[0] = CreateNode(NODE_FUNCTION_ARG);
    // First arg
    node->function_args[0]->arg_name = parser->current_token->value;
    Parser_Expect(parser, TOKEN_ID); // Args in function decl may only consist of identifiers
    Parser_Expect(parser, TOKEN_ARROW);
    Parser_Expect(parser, TOKEN_ID); // Arg type

    if (strcmp(parser->current_token->value, "Array") == 0)
    {
        Parser_Expect(parser, TOKEN_LBRACKET); // [
        Parser_Expect(parser, TOKEN_ID); // TYPE
        node->function_args[0]->arg_type = malloc(strlen(parser->prev_token->value) + strlen("-arr") + 1);
        node->function_args[0]->arg_type = strcat(parser->prev_token->value, "-arr");
        Parser_Expect(parser, TOKEN_RBRACKET); // ]
    }
    else // its not array
    {
        node->function_args[0]->arg_type = parser->prev_token->value;
    }

    node->function_args_size = 1;

    // If we only have one arg
    if (parser->current_token->type == TOKEN_RPAREN) // (
        return Parser_ParseFunctionBody(parser, node); // Parse body

    if (parser->current_token->type != TOKEN_COMMA)
    {
        Error("Expected ',' or ')' but got '%s' in line %d", parser->current_token->value, parser->lexer->lineno);
        exit(1);
    }

    while (parser->current_token->type == TOKEN_COMMA)
    {
        node->function_args_size += 1;

        Parser_Next(parser); // Skip comma
        Parser_Expect(parser, TOKEN_ID); // name of function

        node->function_args = realloc(node->function_args, node->function_args_size * sizeof(struct Node*));
        node->function_args[node->function_args_size - 1] = CreateNode(NODE_FUNCTION_ARG);

        node->function_args[node->function_args_size - 1]->arg_name = parser->prev_token->value;
        Parser_Expect(parser, TOKEN_ARROW); // ->

        if (strcmp(parser->current_token->value, "Array") == 0)
        {
            Parser_Next(parser);
            Parser_Expect(parser, TOKEN_LBRACKET); // [
            Parser_Expect(parser, TOKEN_ID); // TYPE
            node->function_args[node->function_args_size - 1]->arg_type = malloc(strlen(parser->prev_token->value) + strlen("-arr") + 1);
            node->function_args[node->function_args_size - 1]->arg_type = strcat(parser->prev_token->value, "-arr");
            Parser_Expect(parser, TOKEN_RBRACKET); // ]
        }
        else
        {
            node->function_args[node->function_args_size - 1]->arg_type = parser->current_token->value;
            Parser_Next(parser); // Now current_token is either ',' or ')' (ideally)
            // If current_token is ',' we will stay in loop else we will exit
        }
    }

    Parser_Expect(parser, TOKEN_RPAREN); // Closing parentethis

    Parser_Expect(parser, TOKEN_ARROW);
    node->function_return_type = parser->current_token->value;
    Parser_Expect(parser, TOKEN_ID); // The type

    return Parser_ParseFunctionBody(parser, node); // Now parse the body
}

Node* Parser_ParseFunctionBody(Parser* parser, Node* node)
{
    Parser_Expect(parser, TOKEN_LBRACE); // Begining of body ('{')
    int bodySize = 0;
    node->function_body = calloc(bodySize, sizeof(struct Node*));
    while (parser->current_token->type != TOKEN_RBRACE) // Not end of body
    {
        node->function_body = realloc(node->function_body, bodySize + 1);
        node->function_body[bodySize] = Parser_ParseStatement(parser);
        bodySize += 1;
        node->function_body_size = bodySize;
    }
    Parser_Expect(parser, TOKEN_RBRACE); // End of function body

    return node;
}

Node* Parser_ParseFunctionCall(Parser* parser)
{
    // prev_token is identifier because this is a statement
    Node* node = CreateNode(NODE_FUNCTION_CALL);
    node->function_call_name = parser->prev_token->value;

    Parser_Next(parser);

    // Same concept as function decl args
    if (parser->current_token->type == TOKEN_RPAREN) // ')' no arguments
    {
        Parser_Expect(parser, TOKEN_RPAREN);
        Parser_Expect(parser, TOKEN_SEMI);
        return node;
    }

    node->function_call_args = calloc(1, sizeof(struct Node*));
    node->function_call_args[0] = CreateNode(NODE_FUNCTION_ARG);

    node->function_call_args[0]->arg_name = parser->current_token->value;

    switch (parser->current_token->type)
    {
    case TOKEN_STRING:
        node->function_call_args[0]->arg_type = "string";
        break;
    case TOKEN_ID:
        node->function_call_args[0]->arg_type = "variable";
        break;
    case TOKEN_INT:
        node->function_call_args[0]->arg_type = "int";
        break;
    default:
        Error(
            "Expected an Identifier, string or int but got type '%d' at line: %d",
            parser->current_token->type,

            parser->lexer->lineno
        );
        exit(1);
    }

    node->function_call_args_size = 1;

    Parser_Next(parser);

    if (parser->current_token->type == TOKEN_RPAREN) // ')' Got all arguments 
    {
        Parser_Next(parser);
        Parser_Expect(parser, TOKEN_SEMI);
        return node;
    }

    if (parser->current_token->type != TOKEN_COMMA)
    {
        Error("Expected ',' or ')' but got '%s' at line %d", parser->current_token->value, parser->lexer->lineno);
        exit(1);
    }

    while (parser->current_token->type == TOKEN_COMMA)
    {
        Parser_Next(parser);

        node->function_call_args_size += 1;
        int i = node->function_call_args_size - 1;

        node->function_call_args = realloc(node->function_call_args, node->function_call_args_size * sizeof(struct Node*));
        node->function_call_args[i] = CreateNode(NODE_FUNCTION_ARG);

        node->function_call_args[i]->arg_name = parser->current_token->value;

        switch (parser->current_token->type)
        {
        case TOKEN_STRING:
            node->function_call_args[i]->arg_type = "string";
            break;
        case TOKEN_ID:
            node->function_call_args[i]->arg_type = "variable";
            break;
        case TOKEN_INT:
            node->function_call_args[i]->arg_type = "int";
            break;
        default:
            Error(
                "Expected an Identifier, string or int but got type '%s' at line: %d",
                parser->current_token->type,
                parser->lexer->lineno
            );
            exit(1);
        }

        Parser_Next(parser);
    }

    Parser_Expect(parser, TOKEN_RPAREN);
    Parser_Expect(parser, TOKEN_SEMI);

    return node;
}

Node* Parser_ParseVariableDecl(Parser* parser)
{
    Node* node = CreateNode(NODE_VARIABLE_DECL);

    node->variable_decl_name = parser->prev_token->value; // NAME
    Parser_Expect(parser, TOKEN_ARROW); // ->
    Parser_Expect(parser, TOKEN_ID); // TYPE
    node->variable_decl_type = parser->prev_token->value;

    if (strcmp(parser->prev_token->value, "Array") == 0)
    {
        Parser_Expect(parser, TOKEN_LBRACKET); // [
        Parser_Expect(parser, TOKEN_ID); // TYPE
        node->function_args[0]->arg_type = strcat(strcat("Array[", parser->prev_token->value), "]");
        Parser_Expect(parser, TOKEN_RBRACKET); // ]
    }
    Parser_Expect(parser, TOKEN_EQUALS); // =

    if (parser->current_token->type != TOKEN_STRING && parser->current_token->type != TOKEN_INT)
    {
        Error("Expected a string or int but got '%s' on line %d", parser->current_token->value, parser->lexer->lineno);
        exit(1);
    }

    node->variable_decl_value = parser->current_token->value;

    Parser_Next(parser); // skip var value
    Parser_Expect(parser, TOKEN_SEMI);

    return node;
}
