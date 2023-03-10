#ifndef Node_H
#define Node_H

#include <stddef.h>
#include <stdlib.h>

/**
 * @brief Stores all necesarry variables for a node
 *
 */
typedef struct Node
{
    enum
    {
        //NODE_STATEMENT,
        NODE_PROGRAM,
        NODE_FUNCTION_DECL,
        NODE_FUNCTION_CALL,
        NODE_FUNCTION_ARG,
        NODE_STRING,
        NODE_VARIABLE_DECL,
        NODE_VARIABLE,
        NODE_NOOP
    } type;

    // Program
    struct Node** program;
    size_t program_size;

    // Function decl
    char* function_name;
    struct Node** function_body;
    size_t function_body_size;
    struct Node** function_args;
    size_t function_args_size;
    char* function_return_type;
    struct Node* function_return_value; // It's a variable

    // Function call
    char* function_call_name;
    struct Node** function_call_args;
    size_t function_call_args_size;

    // Function arg
    char* arg_name;
    char* arg_type;

    // String
    char* string_value;

    // Variable decl
    char* variable_decl_name;
    char* variable_decl_type;
    char* variable_decl_value;

    // Variable 
    char* variable_name;
} Node;

/**
 * @brief Constructor for node
 *
 *
 * @param type
 * @return
 */
Node* CreateNode(unsigned int type);

#endif