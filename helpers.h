
#ifndef ENDPROJECT_HELPERS_H
#define ENDPROJECT_HELPERS_H

#include <stdlib.h>
#include "ctype.h"
#include "content_validation.h"

#define Skip_Space(str) while(*(str)!='\0' && isspace(*(str)))\
                                (str)++;

/**
 * Wrapper for the strok method, keeps the string unmutated
 * @param str String to be breaked into tokens
 * @param delim Delimiter to the next word
 * @return a pointer to the last token found in the string. A null pointer is returned if there are no tokens left to retrieve.
 */
char *safe_strtok(char *str,const char *delim);

/**
 * Wrapper for the malloc method. validates memory allocation and prints error if needed
 * @param size Size of type
  */
void *safe_malloc(size_t size);

/**
 * Converts binary to weird-4 base
 * 00 - a
 * 01 - b
 * 10 - c
 * 11 - d
 * @param bin
 * @param fourBaseWord
 * @param nmems
 */
void bin_to_weird4(unsigned int bin, char *fourBaseWord, unsigned nmems);

/**
 * Copies the matrix args into arg1 and arg2
 * @param mat the matrix as string
 * @param arg1 the first arg initializer as string
 * @param arg2 the 2nd arg initializer as string
 */
void cpy_mat_vals(const char *mat,char *arg1,char *arg2);

/**
 * Gets the operand address mode
 * @param op
 * @return the op address mode(e.g IMMEDIATE,DIRECT,REGISTER etc)
 */
AddressModeType get_add_mode(char *op);

#endif /*ENDPROJECT_HELPERS_H*/
