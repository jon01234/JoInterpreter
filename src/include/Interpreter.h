#ifndef Interpreter_H
#define Interpreter_H

#include "Node.h"
#include "Print.h"
#include <string.h>

typedef struct Interpreter
{
    Node** functions;
    Node** variables;
} Interpreter;

/**
 * @brief Create a Interpreter object
 *
 * @return Interpreter*
 */
Interpreter* CreateInterpreter();

void Interpret_Statement(Interpreter* interpreter, Node* statement);
void Interpret_FunctionDecl(Interpreter* interpreter, Node* function);
void Interpret_FunctionCall(Interpreter* interpreter, Node* function);
void Interpret_VariableDecl(Interpreter* interpreter, Node* variable);
/**
 * @brief It itreprets when someone uses predefined variable
 *
 * @param variable
 */
void Interpret_Variable(Interpreter* interpreter, Node* variable);

#endif