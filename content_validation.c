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

#ifndef ENDPROJECT_DATA_STRUCT_H
    #include "data_struct.h"
#endif


void printerr(char *lineContent,char *str,int lineNum){

    fprintf(stderr,"Error has occurred at line %d, Please examine:\n%s\n",lineNum,str);

}

int validLabel(char *label){
    int i,slen;
    if((slen = strlen(label)) > 30 || isDsm(label) || IS_EXTERNAL(label) || isCmd(label))
        return FALSE;
    for(i = 0; i < slen-1 ;i++)
        if(!isalnum(label[i]))
            return FALSE;

}

int isDsm(char *word){
    if(strcmp(word,"data") == 0 || strcmp(word,"string") == 0 || strcmp(word,"mat") == 0)
        return TRUE;

    return FALSE;
}

int isCmd(char *word){
    int i = 0;
    extern const struct COMMAND const COMMANDS[NUM_OF_CMDS];
    for(i = 0;i<NUM_OF_CMDS;i++)
        if(strcmp(COMMANDS[i].cmd,word) == 0)
            return TRUE;

    return FALSE;
}