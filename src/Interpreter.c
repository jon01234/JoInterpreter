#include "include/Interpreter.h"
#include "include/Print.h"
#include "include/Utils.h"
#include <stdlib.h>
#include <string.h>

Interpreter* CreateInterpreter()
{
    Interpreter* interpreter = calloc(1, sizeof(Interpreter));

    interpreter->function_count = 0;
    interpreter->functions = NULL;
    interpreter->function_names = NULL;
    interpreter->variable_count = 0;
    interpreter->variables = NULL;
    interpreter->variable_names = NULL;

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
    // Check if function already exists
    qsort(interpreter->function_names, interpreter->function_count, sizeof(char*), cmp_string);
    unsigned int function_exist = sorted_string_search(
            interpreter->function_names,
            interpreter->function_count,
            function->function_name
        );

    // In that case throw error
    if (function_exist != -1)
    {
        Error("Function '%s' already exists", function->function_name);
        exit(1);
    }
    
    
    const Node* last_node = function->function_body[function->function_body_size - 1];
    // We are not on the return and it's the end
    if (!(last_node->type == NODE_VARIABLE_DECL && strcmp(last_node->variable_decl_name, "return") == 0))
    {
        Error("Every function must end with a return statement");
        exit(1);
    }
    
    interpreter->function_count += 1;
    // Realloc problem isn't here
    interpreter->functions = realloc(interpreter->functions, sizeof(interpreter->functions) * interpreter->function_count);
    interpreter->function_names = realloc(interpreter->function_names, sizeof(interpreter->function_names) * interpreter->function_count);

    interpreter->functions[interpreter->function_count - 1] = function;
    interpreter->function_names[interpreter->function_count - 1] = function->function_name;
}

void Interpret_FunctionCall(Interpreter* interpreter, Node* function)
{
    // Lookup if function name exist in interpreter
    qsort(interpreter->function_names, interpreter->function_count, sizeof(char*), cmp_string);
    unsigned int function_index = sorted_string_search(
            interpreter->function_names, 
            interpreter->function_count, 
            function->function_call_name
        );

    if (function_index == -1)
    {
        Error("Cannot find function: '%s'", function->function_call_name);
        exit(1);
    }

    // Interpret everything in the function
    Node* func = interpreter->functions[function_index];
    for (int i = 0; i < func->function_body_size; i++)
    {
        Node* current_node = func->function_body[i];
        
        // Interpret whole body including return statement 
        // because it is a variable that can be interpret with interpret variable decl
        Interpret_Statement(interpreter, current_node);
    }
}

void Interpret_VariableDecl(Interpreter* interpreter, Node* variable)
{
    // Check if variable already exists 
    // In that case throw error
    //
    // Add variable to interpreter
    // Check if it's return (Do some special stuff with it)
    // TODO: Think what these special stuff are 
}

void Interpret_Variable(Interpreter* interpreter, Node* variable)
{
    // Lookup if variable name exists in interpreter
    // TODO: Think what I should do next
}
