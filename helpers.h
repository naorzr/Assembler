
#ifndef ENDPROJECT_HELPERS_H
#define ENDPROJECT_HELPERS_H

#include <stdlib.h>
#include "ctype.h"
#include "content_validation.h"

#define Skip_Space(str) while(*(str)!='\0' && isspace(*(str)))\
                                (str)++;


/* my own version of strtok that does not mutate the given string */
char *safe_strtok(char *str,const char *delim);


void *safe_malloc(size_t size);


void bin_to_weird4(unsigned int bin, char *fourBaseWord, unsigned nmems);


AddressModeType get_add_mode(char *op);


int cpy_mat_vals(const char *mat,char *arg1,char *arg2);

#endif /*ENDPROJECT_HELPERS_H*/
