//
// Created by naortif on 8/9/17.
//

#ifndef ENDPROJECT_HELPERS_H
#define ENDPROJECT_HELPERS_H

#include <stdlib.h>
#include <string.h>
#include "ctype.h"

#define Skip_Space(str) while(*(str)!='\0' && isspace(*(str)))\
                                (str)++;


#define a 0
#define b 1
#define c 2
#define d 3


/* my own version of strtok that does not mutate the given string */
char *safe_strtok(char *str,char *delim);


void *safe_malloc(int nmemb,size_t size);


void binToWierdFour(unsigned int bin,char *fourBaseWord,unsigned nmems);

#endif //ENDPROJECT_HELPERS_H
