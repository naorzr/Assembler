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
    if (argc < 2) {
        LOG_TRACE(LOG_ERROR, "No Files Were Passed\n");
        return 1;
    }
    FILE *inpf = fopen(argv[1],"r");
    parse_file(inpf,FIRST_PASS);
         /* helper method, wont go into the actual code */
    parse_file(inpf,SECOND_PASS);
    test("complete",argv[1],"secondpass");

//    rewind(inpf);
//    do {
//        str = getword(inpf);
//        if (str[0] != EOF)
//            printf("%s\n", str);
//    } while (str[0] != EOF);

    return 0;
}
