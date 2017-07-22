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
    char *operands;
    char *op1;
    char *op2;
    char comma[4] = " ,\t";
    int wordSize = (int) strlen(word);

    if (word[wordSize - 1] == ':') { /* line begins with label */
        strncpy(fileLine->label, word, wordSize);
        fileLine->label[wordSize - 1] = (char) 0;
        word = strtok(NULL, " \t");
        if (word != NULL) {
            strcpy(fileLine->command, word);
            operands = strtok(NULL, " \t");
            if(word[0] == '.') { /* command is .data/.string/.mat */
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

    } else { /* command with no label in line (e.g. .entry/.extern) */
        strncpy(fileLine->command, word, wordSize);
        operands = strtok(NULL, " \t");
        op1 = strtok(operands, comma);
        if (op1 != NULL) {
            strcpy(fileLine->op1, op1);
        }
        op2 = strtok(NULL, comma);
        if (op2 != NULL) {
            strcpy(fileLine->op2, op2);
        }
    }

    LOG_TRACE(LOG_DEBUG,
              "[DEBUG] line content: %s,\n\tlabel: %s,\n\tcommand: %s,\n\top1: %s,\n\top2: %s\n",
              fileLine->lineContent, fileLine->label, fileLine->command,
              fileLine->op1, fileLine->op2);

}

FileContent *initFileContent() {
    FileContent *fileContent = (FileContent *) malloc(sizeof(FileContent));
    if (!fileContent) {
        LOG_TRACE(LOG_ERROR, "Failed to allocate memory for the FileContent structure");
        return NULL;
    }
    memset(fileContent, 0, sizeof(FileContent));
    FileLine * line = (FileLine *) malloc(sizeof(FileLine));

    if (!line) {
        LOG_TRACE(LOG_ERROR, "Failed to allocate memory for the FileLine structure");
        return NULL;
    }
    memset(line, 0, sizeof(FileLine));
    fileContent->lines = line;

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
                  fileContent->lines[i].lineContent, fileContent->lines[i].lineNo, fileContent->lines[i].label, fileContent->lines[i].command,
                  fileContent->lines[i].op1, fileContent->lines[i].op2);
    }
    LOG_TRACE(LOG_DEBUG,"[INFO] END - printFileContent\n");
}


void buildFileContent(FILE *inpf) {
    fileContent = initFileContent();
    if (!fileContent)
        exit(0);

    int lineNumber = 0;
    char lineContent[MAX_LINE];
    while (fgets(lineContent, sizeof(lineContent), inpf) != NULL) {
        lineNumber++;
        /* if line is comment or empty - skip */
        if (lineContent[0] == ';' || lineContent[0] == '\n') {
            continue;
        }
        fileContent->noOfLines++;

        if (fileContent->noOfLines >= 1) {
            (*fileContent).lines = (FileLine *) realloc((*fileContent).lines,
                                                        sizeof(FileLine) * (fileContent->noOfLines));
            FileLine *line = (FileLine *) malloc((sizeof(FileLine)));
            if (!(*fileContent).lines || !line) {
                LOG_TRACE(LOG_ERROR, "Failed to allocate memory for the FileLine structure");
                exit(0);
            }

            memset(line, 0, sizeof(FileLine));
            fileContent->lines[fileContent->noOfLines-1] = *line;
        }


        strcpy(fileContent->lines[fileContent->noOfLines-1].lineContent, lineContent);
        int lineSize = (int) strlen(lineContent);
        if (fileContent->lines[fileContent->noOfLines-1].lineContent[lineSize - 1] == '\n')
            fileContent->lines[fileContent->noOfLines-1].lineContent[lineSize - 1] = (char) 0;
        fileContent->lines[fileContent->noOfLines-1].lineNo = lineNumber;

        parseLine(&(fileContent->lines[fileContent->noOfLines-1]));
    }
    printFileContent(fileContent);


}



