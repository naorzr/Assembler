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

/* my own version of strtok that does not mutate the given string */
char *safe_strtok(char *str,char *delim);

#endif //ENDPROJECT_HELPERS_H
