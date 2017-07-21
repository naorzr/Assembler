#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINE 80
#define SPACE ' '
#define NEWLINE '\n'
#define COMMA ','

char *getword(FILE *inpf){
    int ch,i = 0;
    char *str = malloc(sizeof(char)*MAX_LINE);

    while(!isspace(ch = getc(inpf)) && ch != EOF && ch != COMMA)
        str[i++] = ch;

    if(ch == NEWLINE || ch == EOF) {
        if (i == 0)
            str[i++] = ch;
        else
            ungetc(ch, inpf);
    }
    str[i] = '\0';
    return str;
}

int main() {
    int i = 0;
    char *str;
    FILE *inpf = fopen("file2.as","r");
    do {
        str = getword(inpf);
        if(str[0] != EOF )
             printf("%s\n",str);
    } while(str[0] != EOF);

    return 0;
}