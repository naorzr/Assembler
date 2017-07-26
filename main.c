#include <ctype.h>
#include "logger.h"
#include "file_content.h"
#include "assembler.h"


char *getword(FILE *inpf) {
    int ch, i = 0;
    char *str = malloc(sizeof(char) * MAX_LINE);

    while (!isspace(ch = getc(inpf)) && ch != EOF && ch != COMMA)
        str[i++] = ch;

    if (ch == NEWLINE || ch == EOF) {
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
    if (argc < 2) {
        LOG_TRACE(LOG_ERROR, "No Files Were Passed\n");
        return 1;
    }
    FILE *inpf = getFileContent(argv[1]);
    buildFileContent(inpf);
    firstIteration();

//    rewind(inpf);
//    do {
//        str = getword(inpf);
//        if (str[0] != EOF)
//            printf("%s\n", str);
//    } while (str[0] != EOF);

    return 0;
}
