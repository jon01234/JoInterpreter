#ifndef JoInterpreter_H
#define JoInterpreter_H

#include "Lexer.h"
#include "Parser.h"
#include "Node.h"
#include "Print.h"
#include "IO.h"
#include "Interpreter.h"

/**
 * @brief Interprets a char* of code
 *
 * @param code char*
 */
void JoIn_Interpret(char* code);
/**
 * @brief Interprets a file
 *
 * @param filename
 */
void JoIn_InterpretFile(char* filename);

#endif
