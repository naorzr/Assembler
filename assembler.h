
#ifndef ENDPROJECT_ASSEMBLER_H
#define ENDPROJECT_ASSEMBLER_H


#include <stdio.h>

#define MAX_LINE 81

enum{FIRST_PASS, SECOND_PASS};
/* TODO: might want to consider making the return type err_t , thus making the error process more natural */

enum ErrorTypes first_pass(FILE *inpf);


#endif //ENDPROJECT_ASSEMBLER_H