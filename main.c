/*
 * Mamman14: This is an assembly program for assembly files
 * @authors:
 * Barak Kinarti
 * Naor Zruk    204424071
 */


#include "logger.h"
#include "assembler.h"
#include "data_struct.h"

/**
 * Gets command line arguments with the assembly file names and interpret them one by one
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
        if (parse_file(inpf, FIRST_PASS) != NO_ERR_OCCURRED) {
            log_trace(LOG_ERROR,
                      "Errors found during the first pass phase of file name: %s, skipping to the next file\n",
                      fileName);
            fclose(inpf);
            continue;
        }
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
