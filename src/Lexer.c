#include "include/Lexer.h"

Lexer* CreateLexer(char* code)
{
    Lexer* lexer = calloc(1, sizeof(Lexer));

    lexer->code = code;
    lexer->code_size = strlen(code);
    lexer->i = 0;
    lexer->current_char = lexer->code[lexer->i];
    lexer->lineno = 1;

    return lexer;
}

void Lexer_Advance(Lexer* lexer)
{
    // Exit the function if the end of the code is reached
    if (lexer->i < lexer->code_size && lexer->current_char == '\0') return;

    lexer->i += 1;
    lexer->current_char = lexer->code[lexer->i];
}

void Lexer_SkipWhitespace(Lexer* lexer)
{
    while (lexer->current_char == ' ' || lexer->current_char == '\t' || lexer->current_char == '\n' || lexer->current_char == '\r')
    {
        if (lexer->current_char == '\n')
            lexer->lineno += 1; // Track all lines to give detailed error messages

        Lexer_Advance(lexer);
    }
}

Token* Lexer_GetIdentifier(Lexer* lexer)
{
    // This should never be reached
    // But in case
    if (!isalpha(lexer->current_char) || lexer->current_char == '_')
    {
        Error("Identifier dosen't start with letter or '_'");
        return NULL;
    }

    char* value = calloc(1, sizeof(char));

    // [a-zA-Z_][a-zA-Z_0-9]* in regex
    while (isalnum(lexer->current_char) || lexer->current_char == '_')
    {
        value = realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, (char[]) { lexer->current_char, 0 });
        Lexer_Advance(lexer);
    }

    return CreateToken(value, TOKEN_ID);
}

Token* Lexer_GetString(Lexer* lexer)
{
    char* value = calloc(1, sizeof(char));

    Lexer_Advance(lexer); // Skip first Qoute

    while (lexer->current_char != '"')
    {
        value = realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, (char[]) { lexer->current_char, 0 });
        Lexer_Advance(lexer);
    }

    Lexer_Advance(lexer); // Advance past last Qoute

    return CreateToken(value, TOKEN_STRING);
}

Token* Lexer_GetNumber(Lexer* lexer)
{
    // Just as the same as the GetIdentifier function but with only numbers 

    char* value = calloc(1, sizeof(char));
    while (isdigit(lexer->current_char))
    {
        value = realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, (char[]) { lexer->current_char, 0 });
        Lexer_Advance(lexer);
    }

    return CreateToken(value, TOKEN_INT);
}

Token* Lexer_GetNextToken(Lexer* lexer)
{
    // '\0' is character for end of file
    while (lexer->current_char != '\0')
    {
        Lexer_SkipWhitespace(lexer);

        if (isalpha(lexer->current_char) || lexer->current_char == '_') // In regex it would be [a-zA-Z_] this is why the statement in get identifier shouldn't be reached
            return Lexer_GetIdentifier(lexer);

        if (isdigit(lexer->current_char)) // Regex [0-9] or \d
            return Lexer_GetNumber(lexer);

        if (lexer->current_char == '"')
            return Lexer_GetString(lexer);

        if (lexer->current_char == '-' && Lexer_PeakNext(lexer) == '>')
        {
            Lexer_Advance(lexer);
            Lexer_Advance(lexer);
            return CreateToken("->", TOKEN_ARROW);
        }

        switch (lexer->current_char)
        {
        case '(': Lexer_Advance(lexer); return CreateToken("(", TOKEN_LPAREN); break;
        case ')': Lexer_Advance(lexer); return CreateToken(")", TOKEN_RPAREN); break;
        case ',': Lexer_Advance(lexer); return CreateToken(",", TOKEN_COMMA); break;
        case '=': Lexer_Advance(lexer); return CreateToken("=", TOKEN_EQUALS); break;
        case '[': Lexer_Advance(lexer); return CreateToken("[", TOKEN_LBRACKET); break;
        case ']': Lexer_Advance(lexer); return CreateToken("]", TOKEN_RBRACKET); break;
        case '{': Lexer_Advance(lexer); return CreateToken("{", TOKEN_LBRACE); break;
        case '}': Lexer_Advance(lexer); return CreateToken("}", TOKEN_RBRACE); break;
        case ';': Lexer_Advance(lexer); return CreateToken(";", TOKEN_SEMI); break;
        case '\0': break;
        default:
            Error("Unknown character '%c' with ASCII value '%d' at line: %d\n", lexer->current_char, lexer->current_char, lexer->lineno);
            Lexer_Advance(lexer);
            return CreateToken("\"UNKNOWN TOKEN\"", TOKEN_UNKNOWN);
            break;
        }
    }

    return CreateToken(NULL, TOKEN_EOF);
}

char Lexer_Peak(Lexer* lexer, int n)
{
    return lexer->i + n < lexer->code_size ? lexer->code[lexer->i + n] : lexer->code[lexer->code_size];
}

char Lexer_PeakNext(Lexer* lexer)
{
    return Lexer_Peak(lexer, 1);
}
