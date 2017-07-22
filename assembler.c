#include <string.h>
#include "file_content.h"
#include "assembler.h"
#include "logger.h"

void firstIteration() {
    int i, commandSize;
    ic = MEMORY_STARTING_ADDRESS;
    dc = 0;

    for (i=0; i < fileContent->noOfLines; i++) { /* For every line in file */
        FileLine line = fileContent->lines[i];
        int hasLabel = 0;
        int dataSize = 0;

        if(strlen(line.label) > 0) {
            hasLabel = 1;
        }

        /* handle commands */

        /* handle external */


        /* handle storage entities */
        if (equals(line.command, DATA) || equals(line.command, MAT)) {
            dataSize = getCommaCount(line.lineContent) + 1;
//            LOG_TRACE(LOG_DEBUG,"encountered .data line: %s, dataSize: %d\n", line.lineContent, dataSize);
            /*TODO: push to data table*/
        } else if(equals(line.command, STRING)) {
            trimStringQuotes(line.op1); /* remove string quotes*/
            dataSize = strlen(line.op1);
//            LOG_TRACE(LOG_DEBUG,"encountered .string line: %s, dataSize: %d\n", line.lineContent, dataSize);
            /*TODO: push to data table*/
        }
        dc += dataSize;
    }
    LOG_TRACE(LOG_DEBUG,"Final Data Size: %d\n",dc);

};

int getCommaCount(char *str) {
    int i,
            counter = 0,
            len = strlen(str);

    for (i = 0; i < len; i++) {
        if (str[i] == ',') {
            counter++;
        }
    }
    return counter;
}

char *trimStringQuotes(char *str) {
    int i, j = 0, len = strlen(str);
    for (i = 0; i < len; i ++) {
        if (str[i] != '"' && str[i] != '\\') {
            str[j++] = str[i];
        } else if (str[i+1] == '"' && str[i] == '\\') {
            str[j++] = '"';
        } else if (str[i+1] != '"' && str[i] == '\\') {
            str[j++] = '\\';
        }
    }

//You missed the string termination ;)
    if(j>0) str[j]=0;
    return str;
}