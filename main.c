#include <ctype.h>
#ifndef ENDPROJECT_LOGGER_H
    #include "logger.h"
#endif
#ifndef ENDPROJECT_FILE_CONTENT_H
    #include "file_content.h"
#endif
#ifndef ENDPROJECT_ASSEMBLER_H
    #include "assembler.h"
#endif


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
    FILE *inpf = fopen(argv[1],"r");
    buildFileContent(inpf);

//    rewind(inpf);
//    do {
//        str = getword(inpf);
//        if (str[0] != EOF)
//            printf("%s\n", str);
//    } while (str[0] != EOF);

    return 0;
}
