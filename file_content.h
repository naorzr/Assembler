#include <stdio.h>
#include <stdlib.h>


#ifndef ENDPROJECT_FILE_CONTENT_H
#define ENDPROJECT_FILE_CONTENT_H

#endif //ENDPROJECT_FILE_CONTENT_H

#define MAX_LINE 81
#define NEWLINE '\n'
#define COMMA ','

enum{FIRST_PASS, SECOND_PASS};
#define Skip_Space(str)  \
while(*(str) != '\0' && isspace(*(str)))\
                (str)++;

void buildFileContent(FILE *);

void parseLine(char *str,int lineNum);
