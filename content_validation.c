//
// Created by naortif on 7/24/17.
//
#include <string.h>
#include <ctype.h>

#ifndef ENDPROJECT_CONTENT_VALIDATION_H
    #include "content_validation.h"
#endif

#ifndef ENDPROJECT_FILE_CONTENT_H
    #include "file_content.h"
#endif


void printerr(char *lineContent,char *str,int lineNum){

    fprintf(stderr,"Error has occurred at line %d, Please examine:\n%s\n",lineNum,str);

}

int validLabel(FileLine *fileLine){
    int i,slen;
    if((slen = strlen(fileLine->label)) > 30)
        return FALSE;
    for(i = 0; i < slen-1 ;i++)
        if(!isalnum(fileLine->label[i]))
            return FALSE;

}

int isDsm(char *word){
    if((strcmp(word,".data")|strcmp(word,".string")|strcmp(word,".mat")) == 0)
        return TRUE;

    return FALSE;
}