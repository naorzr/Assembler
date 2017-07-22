#include "file_content.h"
#include "assembler.h"

void firstIteration() {
    int i, commandSize;
    ic = MEMORY_STARTING_ADDRESS;
    dc = 0;

    for (i=0; i < fileContent->noOfLines; i++) { /* For every line in file */
        FileLine line = fileContent->lines[i];
        int hasLabel = 0;

        if(line.label) {
            hasLabel = 1;
        }

        /* handle commands */

        /* handle external */


        /* handle storage */
    }
};