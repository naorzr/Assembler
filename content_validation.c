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

int isLabel(char *label){
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


int isString(char *str){
    unsigned len = (unsigned) strlen(str);
    if(len<3)   /* since quotes takes up two places, the string must be greater than or equal to two */
        return FALSE;

    if(str[0] == '\"' && str[len-1] == '\"')
        return TRUE;

    return FALSE;
}


int isNum(char *str){
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

int cpyMatVals(char *mat,char *arg1,char *arg2){
    char *op1,*op2;
    SKIP_SPACE(mat)

    if(valid_parentheses(mat) == FALSE)
        return FALSE;

    op1 = strtok(mat,"]");
    if(op1 == NULL)
        return FALSE;

    op2 = strtok(NULL,"[");
    if(op2 == NULL)
        return FALSE;

    SKIP_SPACE(op2)
    op2[strlen(op2)-1] = '\0';

    strcpy(arg1,op1);
    strcpy(arg2,op2);

    return TRUE;
}

int isValidMatVal(char *val){
    if(isNum(val) || isareg(val))
        return TRUE;
    return FALSE;
}

int valid_parentheses(char *str){
    char *left,*right;
    int i,k;
    unsigned len
    SKIP_SPACE(str)

    len = strlen(str);

    if(*str != '[' || str[len-1] != ']')
        return FALSE;

    left = str;
    while(++left < str + len){
        if(*left == ']')
            break;
    }
    right = str + len -1;
    while(--right > str){
        if(*right == '[')
            break;
    }

    if(right - left == 1)
        return TRUE;

    return FALSE;
}

int isValidMat(char *str){
    char arg1[MAX_LINE], arg2[MAX_LINE];

    if(cpyMatVals(str,arg1,arg2) == TRUE)
         return isValidMatVal(arg1) && isValidMatVal(arg2);

}
