#include <ctype.h>

#include "logger.h"
#include "assembler.h"
#include "data_struct.h"
#include "content_validation.h"
#include "error_handler.h"

int main(int argc, char **argv) {
    char fileName[MAX_FILE_NAME];
    int i = 0;
    FILE *inpf;
    if (argc < 2) {
        LOG_TRACE(LOG_ERROR, "No Files Were Passed\n");
        return 1;
    }
    for (i = 1; i < argc; i++) {
        strcat(strcpy(fileName, argv[i]), AS_EXT);        /* copies file name with the input postfix(".as") */
        if ((inpf = fopen(fileName, "r")) == NULL) {
            LOG_TRACE(LOG_ERROR, "Error reading file: %s\n", fileName);
            continue;
        }

        if (parse_file(inpf, FIRST_PASS) != NO_ERR_OCCURRED) {
            LOG_TRACE(LOG_ERROR, "Errors found during the first pass phase of file name: %s, skipping to next file\n", fileName);
            continue;
        }
        if (parse_file(inpf, SECOND_PASS) != NO_ERR_OCCURRED) {
            LOG_TRACE(LOG_ERROR, "Errors found during the second pass phase of file name: %s, skipping to next file\n", fileName);
            continue;
        }
        fclose(inpf);
        export_assembly_files(strcpy(fileName, argv[i]));
        /* to use the tester, create 3 files that should be tested against. with a postfix of .test
         * for example filename.as.test or filename.ent.test */
        test(argv[1]);
    }

    LOG_TRACE(LOG_INFO, "Program Ended\n");
    return 0;
}
