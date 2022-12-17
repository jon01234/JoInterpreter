#include "include/JoInterpreter.h"

void JoIn_Interpret(char* code)
{
    Lexer* lexer = CreateLexer(code);
    Parser* parser = CreateParser(lexer);
    Interpreter* interpreter = CreateInterpreter();

    Node* program = Parser_Parse(parser);

    for (int i = 0; i < program->program_size; i++)
    {
        Interpret_Statement(interpreter, program->program[i]);
    }
}

void JoIn_InterpretFile(char* filename)
{
    JoIn_Interpret(IO_ReadFile(filename));
}