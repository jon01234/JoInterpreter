#include "include/Interpreter.h"
#include <stdlib.h>

Interpreter* CreateInterpreter()
{
    Interpreter* interpreter = calloc(1, sizeof(Interpreter));

    interpreter->function_count = 0;
    interpreter->functions = NULL;
    interpreter->variable_count = 0;
    interpreter->variables = NULL;

    return interpreter;
}

void Interpret_Statement(Interpreter* interpreter, Node* statement)
{
    switch (statement->type)
    {
    case NODE_FUNCTION_DECL: return Interpret_FunctionDecl(interpreter, statement); break;
    case NODE_FUNCTION_CALL: return Interpret_FunctionCall(interpreter, statement); break;
    case NODE_VARIABLE_DECL: return Interpret_VariableDecl(interpreter, statement); break;
    case NODE_VARIABLE: return Interpret_Variable(interpreter, statement); break;

    default:
        return;
    }
}

void Interpret_FunctionDecl(Interpreter* interpreter, Node* function)
{
    const Node* last_node = function->function_body[function->function_body_size - 1];
    // We are not on the return and it's the end
    if (!(last_node->type == NODE_VARIABLE_DECL && strcmp(last_node->variable_decl_name, "return") == 0))
    {
        Error("Every function must end with a return statement");
    }
    
    interpreter->function_count += 1;
    interpreter->functions = realloc(interpreter->functions, interpreter->function_count);

    interpreter->functions[interpreter->function_count] = function;
}

void Interpret_FunctionCall(Interpreter* interpreter, Node* function)
{
    // Lookup if function name exist in interpreter
    // Parse everything in the function
}

void Interpret_VariableDecl(Interpreter* interpreter, Node* variable)
{
    // Add variable to interpreter
}

void Interpret_Variable(Interpreter* interpreter, Node* variable)
{
    // Lookup if variable name exists in interpreter
    // TODO: Think what i should do next
}
