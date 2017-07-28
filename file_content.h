#include <stdio.h>
#include <stdlib.h>


#ifndef ENDPROJECT_FILE_CONTENT_H
#define ENDPROJECT_FILE_CONTENT_H

#endif //ENDPROJECT_FILE_CONTENT_H
enum{OFF,ON};
enum{NOT_AVILABLE = -1, NONE,EXTERN,ENTRY,NO, YES};

#define MAX_LINE 81
#define NEWLINE '\n'
#define COMMA ','


void buildFileContent(FILE *);

void parseLine(char *str,int lineNum);
