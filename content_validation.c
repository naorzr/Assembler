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


void printerr(FileLine *fileLine,char *str){

    fprintf(stderr,"Error has occurred at line %d, Please examine:\n%s\n",fileLine->lineNum,str);

}

int validLabel(FileLine *fileLine){
    int i,slen;
    if((slen = strlen(fileLine->label)) > 30)
        return FALSE;
    for(i = 0; i < slen-1 ;i++)
        if(!isalnum(fileLine->label[i]))
            return FALSE;

}