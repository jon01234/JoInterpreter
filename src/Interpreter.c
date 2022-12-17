#include "include/Interpreter.h"

Interpreter* CreateInterpreter()
{
    return NULL;
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
    for (int i = 0; i < function->function_body_size; i++)
    {
        Node* current_node = function->function_body[i];

        if (i != function->function_body_size - 1) // Not the end of function
        {
            Interpret_Statement(interpreter, current_node);
        }
        // We are on the return
        else if (i == function->function_body_size - 1 && current_node->type == NODE_VARIABLE_DECL && strcmp(current_node->variable_decl_name, "return") == 0)
        {
            function->function_return_value = current_node;
        }
        else
        {
            Error("Every function must end with a return statement");
        }
    }
}

void Interpret_FunctionCall(Interpreter* interpreter, Node* function)
{

}

void Interpret_VariableDecl(Interpreter* interpreter, Node* variable)
{

}

void Interpret_Variable(Interpreter* interpreter, Node* variable)
{

}