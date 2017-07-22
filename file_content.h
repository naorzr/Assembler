#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE 80
#define NEWLINE '\n'
#define COMMA ','

typedef struct {
    char lineContent[MAX_LINE];
    char label[MAX_LINE];
    char command[MAX_LINE];
    char op1[MAX_LINE];
    char op2[MAX_LINE];
    int lineNo;
} FileLine;

typedef struct {
    FileLine *lines;
    int noOfLines;
} FileContent;


FILE *getFileContent(char *);

void buildFileContent(FILE *);

FileContent *initFileContent();

void parseLine(FileLine *);

void printFileContent(FileContent *fileContent);

FileContent *fileContent;