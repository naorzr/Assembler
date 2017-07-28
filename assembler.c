#include <string.h>

#ifndef ENDPROJECT_FILE_CONTENT_H
    #include "file_content.h"
#endif
#ifndef ENDPROJECT_ASSEMBLER_H
    #include "assembler.h"
#endif
#ifndef ENDPROJECT_LOGGER_H
    #include "logger.h"
#endif




int getCommaCount(char *str) {
    int i,
            counter = 0,
            len = strlen(str);

    for (i = 0; i < len; i++) {
        if (str[i] == ',') {
            counter++;
        }
    }
    return counter;
}

char *trimStringQuotes(char *str) {
    int i, j = 0, len = strlen(str);
    for (i = 0; i < len; i ++) {
        if (str[i] != '"' && str[i] != '\\') {
            str[j++] = str[i];
        } else if (str[i+1] == '"' && str[i] == '\\') {
            str[j++] = '"';
        } else if (str[i+1] != '"' && str[i] == '\\') {
            str[j++] = '\\';
        }
    }

//You missed the string termination ;)
    if(j>0) str[j]=0;
    return str;
}