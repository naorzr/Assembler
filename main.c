#include <ctype.h>

#include "logger.h"
#include "assembler.h"
#include "data_struct.h"
#include "content_validation.h"
#include "error_handler.h"

char *getword(FILE *inpf) {
    int ch, i = 0;
    char *str = malloc(sizeof(char) * MAX_LINE);

    while (!isspace(ch = getc(inpf)) && ch != EOF && ch != ',')
        str[i++] = ch;

    if (ch == '\n' || ch == EOF) {
        if (i == 0)
            str[i++] = ch;
        else
            ungetc(ch, inpf);
    }
    str[i] = '\0';
    return str;
}

int main(int argc, char **argv) {

    char *str;
    char fileName[MAX_FILE_NAME];
    int i = 0;
    FILE *inpf;
    if (argc < 2) {
        LOG_TRACE(LOG_ERROR, "No Files Were Passed\n");
        return 1;
    }
    for(i = 1; i < argc; i++) {
        strcat(strcpy(fileName,argv[i]),IN_POSTFIX);        /* copies file name with the input postfix(".as") */
        if((inpf = fopen(fileName, "r")) == NULL)
            continue;
        if(parse_file(inpf, FIRST_PASS) != NO_ERR_OCCURRED)
            continue;
        if(parse_file(inpf, SECOND_PASS) != NO_ERR_OCCURRED)
            continue;
        fclose(inpf);
        export_assembly_files(strcpy(fileName,argv[i]));

        test("complete", argv[1], "secondpass");

    }

    return 0;
}
