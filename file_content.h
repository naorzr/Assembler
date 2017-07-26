#include <stdio.h>
#include <stdlib.h>


#ifndef ENDPROJECT_FILE_CONTENT_H
#define ENDPROJECT_FILE_CONTENT_H

#endif //ENDPROJECT_FILE_CONTENT_H


#define MAX_LINE 81
#define NEWLINE '\n'
#define COMMA ','
#define CurFileLine fileLine[fileContent->noOfLines-1]
typedef struct {
    char lineContent[MAX_LINE];
    char label[MAX_LINE];
    char command[MAX_LINE];
    char op1[MAX_LINE];
    char op2[MAX_LINE];
    int lineNum;
} FileLine;

typedef struct {
    FileLine *fileLine;
    int noOfLines;
} FileContent;


FILE *getFileContent(char *);

void buildFileContent(FILE *);

FileContent *initFileContent();

void parseLine(FileLine *);

void printFileContent(FileContent *fileContent);

FileContent *fileContent;