#ifndef IO_H
#define IO_H

/**
 * @brief Read a file and return a char with file text
 *
 * @param filename char*
 * @return The text in the file
 */
char* IO_ReadFile(char* filename);
/**
 * @brief Write a file
 *
 * @param filename char*
 * @param text  char*
 */
void IO_WriteFile(char* filename, char* text);

#endif