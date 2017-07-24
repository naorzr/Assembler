#include <string.h>
#include "file_content.h"
#include "logger.h"

/**
 * Parse a single line into the file content structure (extract labels and commands and operands)
 * @param fileLine
 */
void parseLine(FileLine *fileLine) {
    char lineContent[MAX_LINE];
    strcpy(lineContent, fileLine->lineContent);
    char *word = strtok(lineContent, " \t");
    char *operands, *op1, *op2;
    char comma[] = " ,\t";
    int wordSize = strlen(word);

    if (word[wordSize - 1] == ':') { /* line begins with label */
        strcpy(fileLine->label, word);
        word = strtok(NULL, " \t");
    }

    if (word != NULL) {
            strcpy(fileLine->command, word);
            operands = strtok(NULL, " \t");
            if(word[0] == '.') { /* directive is .data/.string/.mat */
                strcpy(fileLine->op1, operands);
                if (strcmp(word, ".mat") == 0) { /* fetch the matrix values */
                    op2 = strtok(NULL, " \t");
                    strcpy(fileLine->op2, op2);
                }

            } else { /* normal command after label*/
                op1 = strtok(operands, comma);
                if (op1 != NULL) {
                    strcpy(fileLine->op1, op1);
                }
                op2 = strtok(NULL, comma);
                if (op2 != NULL) {
                    strcpy(fileLine->op2, op2);
                }
            }
        }



    LOG_TRACE(LOG_DEBUG,
              "[DEBUG] line content: %s,\n\tlabel: %s,\n\tcommand: %s,\n\top1: %s,\n\top2: %s\n",
              fileLine->lineContent, fileLine->label, fileLine->command,
              fileLine->op1, fileLine->op2);

}

/* initFileContent: allocate memory for a new fileContent structure and initialize all values to Null */
FileContent *initFileContent(void) {
    FileContent *fileContent = (FileContent *) calloc(1,sizeof(FileContent));
    if (!fileContent) {
        LOG_TRACE(LOG_ERROR, "Failed to allocate memory for the FileContent structure");
        return NULL;
    }
    FileLine * line = (FileLine *) calloc(1,sizeof(FileLine));
    if (!line) {
        LOG_TRACE(LOG_ERROR, "Failed to allocate memory for the FileLine structure");
        return NULL;
    }
    fileContent->fileLine = line;

    return fileContent;
}

FILE *getFileContent(char *filename) {
    FILE *inpf = fopen(filename, "r");

    if (inpf == NULL) {
        LOG_TRACE(LOG_ERROR, "Could not open file: %s\n", filename);
        exit(0);
    }
    return inpf;
}

void printFileContent(FileContent *fileContent) {
    int i;
    LOG_TRACE(LOG_DEBUG,"[DEBUG] START - printFileContent\n");

    for (i = 0; i <= fileContent->noOfLines-1 ; ++i) {

        LOG_TRACE(LOG_INFO,
                  "[INFO] line content: %s,\n\tline number: %d,\n\tlabel: %s,\n\tcommand: %s,\n\top1: %s,\n\top2: %s\n",
                  fileContent->fileLine[i].lineContent, fileContent->fileLine[i].lineNum, fileContent->fileLine[i].label, fileContent->fileLine[i].command,
                  fileContent->fileLine[i].op1, fileContent->fileLine[i].op2);
    }
    LOG_TRACE(LOG_DEBUG,"[INFO] END - printFileContent\n");
}

/* parse all the data in the input file into the fileContent structure */
void buildFileContent(FILE *inpf) {
    int lineNumber = 0;

    fileContent = initFileContent();
    if (!fileContent)
        exit(EXIT_FAILURE);

    char lineContent[MAX_LINE];
    while (fgets(lineContent, sizeof(lineContent), inpf) != NULL) {
        lineNumber++;
        /* if line is comment or empty - skip */
        if (lineContent[0] == ';' || lineContent[0] == '\n') {
            continue;
        }
        fileContent->noOfLines++;

        if (fileContent->noOfLines >= 1) {
            fileContent->fileLine = (FileLine *) realloc(fileContent->fileLine,
                                                        sizeof(FileLine) * (fileContent->noOfLines));
            FileLine *line = (FileLine *) calloc(1,(sizeof(FileLine)));

            if (!fileContent->fileLine || !line) {
                LOG_TRACE(LOG_ERROR, "Failed to allocate memory for the FileLine structure");
                exit(EXIT_FAILURE);
            }

            fileContent->CurFileLine = *line;
        }


        strcpy(fileContent->CurFileLine.lineContent, lineContent);
        int lineSize = strlen(lineContent);
        if (fileContent->CurFileLine.lineContent[lineSize - 1] == '\n')
            fileContent->CurFileLine.lineContent[lineSize - 1] = '\0';
        fileContent->CurFileLine.lineNum = lineNumber;

        parseLine(&(fileContent->CurFileLine));
    }
    printFileContent(fileContent);


}



