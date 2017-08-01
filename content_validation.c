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


int isastring(char *str){
    unsigned len = (unsigned) strlen(str);
    if(len<3)   /* since quotes takes up two places, the string must be greater than or equal to two */
        return FALSE;

    if(str[0] == '\"' && str[len-1] == '\"')
        return TRUE;

    return FALSE;
}


int isanum(char *str){
    if(*str == '-' || *str == '+' || isdigit(*str))
        while(*(++str) != '\0')
            if(!isdigit(*str))
                return FALSE;
    return TRUE;
}

int isareg(char *op){
    extern const char * const REGISTERS[NUM_OF_REG];
    int i;
    for(i = 0;i < NUM_OF_REG;i++)
        if(strcmp(op,REGISTERS[i]) == 0)
            return TRUE;

    return FALSE;
}

int getmatarg(char *mat,char *arg1,char *arg2){
    char *op1,*op2;
    while(isspace(*mat) && *mat != '\0')
        mat++;
    if(*(mat++) != '[')
        return FALSE;

    op1 = strtok(mat,"]");
    if(op1 == FALSE)
        return FALSE;

    op2 = strtok(NULL,"[");
    if(op2 == NULL || op2[strlen(op2)-1] != ']' || strtok(NULL,"") != NULL)
        return FALSE;
    op2[strlen(op2)-1] = '\0';


    strcpy(arg1,op1);
    strcpy(arg2,op2);

    return TRUE;
}


int validmatvalue(char *val){
    if(isanum(val) || isareg(val))
        return TRUE;
    return FALSE;
}

/* *******not sure wether to use this function or not*********** */
int isamat(char *str){
    char matfmt[] = "[][]";
    int i,k;
    unsigned len;
    len = strlen(str);
    for(i = 0,k = 0;i < len, *(matfmt++) != '\0';i++){

    }
}
