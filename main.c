/*
 * Mamman14
 * Program purpose:
 * This program receives file input from the user and translating them into weird base 4 code.
 * exporting them into 3 different files.
 *
 * General program flow:
 * program parses all file names received in the command line, parsing each line separately - translating the code into
 * weird 4 base and storing it in the right data structure.
 * for this goal 3 data structures were designed with their matching functions.
 * Symbol Table binary search tree - containing nodes of symbols data.
 * Data array - containing all the translated code of the directive sentences.
 * Code Array - containing all the translated code of the command sentences.
 *
 * @authors:
 * Barak Kinarti
 * Naor Zruk
 */


#include "logger.h"
#include "assembler.h"
#include "data_struct.h"

/**
 * Gets command line arguments with the assembly file names and interpret them one by one
 * converting them into weird 4 base code and storing them in external files
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char **argv) {
    char fileName[MAX_FILE_NAME];
    int i = 0;
    FILE *inpf;
    if (argc < 2) {
        log_trace(LOG_ERROR, "No Files Were Passed\nPlease pass a file name, files should contain a %s postfix\n",
                  IN_EXT);
        return 1;
    }
    for (i = 1; i < argc; i++) {
        strcat(strcpy(fileName, argv[i]), IN_EXT);        /* copies file name with the input postfix(".as") */
        if ((inpf = fopen(fileName, "r")) == NULL) {
            log_trace(LOG_ERROR,
                      "Error could not open file: %s\nPlease make sure you passed a valid file name and that the file has %s ending\n",
                      argv[i], IN_EXT);
            continue;
        }
        /* first pass parsing */
        if (parse_file(inpf, FIRST_PASS) != NO_ERR_OCCURRED) {
            log_trace(LOG_ERROR,
                      "Errors found during the first pass phase of file name: %s, skipping to the next file\n",
                      fileName);
            fclose(inpf);
            continue;
        }
        /* second pass parsing */
        if (parse_file(inpf, SECOND_PASS) != NO_ERR_OCCURRED) {
            log_trace(LOG_ERROR,
                      "Errors found during the second pass phase of file name: %s, skipping to the next file\n",
                      fileName);
            fclose(inpf);
            continue;
        }
        fclose(inpf);
        export_assembly_files(strcpy(fileName, argv[i]));

    }

    log_trace(LOG_INFO, "Program Ended\n");
    return 0;
}
