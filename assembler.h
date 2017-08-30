#ifndef ENDPROJECT_ASSEMBLER_H
#define ENDPROJECT_ASSEMBLER_H


#include <stdio.h>
#include "error_handler.h"

#define MAX_LINE 81

/* Enums for the assembler pass indicators */
enum{FIRST_PASS, SECOND_PASS};

/**
 * Takes in a file and parses all the content line by line, translating it to binary code and storing it in the relevant data structs
 * @param inpf file input stream
 * @param passage Type of passage (first/second)
 * @return In case of an error returns the specific type of error
 */
ErrorTypes parse_file(FILE *inpf,int passage);

void export_assembly_files(char *outName);

#endif /*ENDPROJECT_ASSEMBLER_H*/