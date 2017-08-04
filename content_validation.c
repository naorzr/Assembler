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
    if((slen = strlen(label)) > 30 || slen == 0  || !isalpha(label[0]) || isDsm(label) || Is_External(label) || Is_Entry(label) || isCmd(label))
        return FALSE;

    for(i = 0; i < slen-1 ;i++)
        if(!isalnum(label[i]))
            return FALSE;

    return TRUE;
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
    if(*str == '-' || *str == '+' || isdigit(*str)) {
        while (*(++str) != '\0')
            if (!isdigit(*str))
                return FALSE;
        return TRUE;
    }
    return FALSE;
}

int isReg(char *op){
    extern const char * const REGISTERS[NUM_OF_REG];
    int i;
    for(i = 0;i < NUM_OF_REG;i++)
        if(strcmp(REGISTERS[i],op) == 0)
            return TRUE;

    return FALSE;
}

int cpyMatVals(char *mat,char *arg1,char *arg2){
    char *op1,*op2;
    char temp_mat[MAX_LINE];
    strcpy(temp_mat,mat);
    Skip_Space(mat)

    if(valid_parentheses(temp_mat) == FALSE)
        return FALSE;

    op1 = strtok(temp_mat,"]");
    if(op1 == NULL)
        return FALSE;

    op2 = strtok(NULL,"[");
    if(op2 == NULL)
        return FALSE;

    Skip_Space(op2)
    op2[strlen(op2)-1] = '\0';

    strcpy(arg1,&op1[1]);
    strcpy(arg2,op2);

    return TRUE;
}

int isValidMatVal(char *val){
    if(isNum(val) || isReg(val))
        return TRUE;
    return FALSE;
}

int valid_parentheses(char *str){
    char *left,*right;
    int i,k;
    unsigned len;
    Skip_Space(str)

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

int isValidMat(char *str) {
    char arg1[MAX_LINE], arg2[MAX_LINE];

    if (cpyMatVals(str, arg1, arg2) == TRUE)
        return (isNum(arg1)||isReg(arg1)) &&
                (isNum(arg2) || isReg(arg2));



}


int getAddMode(char *op){
    if(op == NULL)
        return ADDMODE_NO_OPERAND;
    else if(op[0] == '#')
        return ADDMODE_IMMEDIATE;
    else if(isLabel(op) == TRUE)
        return ADDMODE_DIRECT;
    else if(isReg(op) == TRUE)
        return ADDMODE_REG;
    else
        return ADDMODE_MATRIX;
}


err_t isValidAddressMode(char *cmd,char *src_op, char *dest_op){
    extern const struct COMMAND const COMMANDS[NUM_OF_CMDS];
    int i = 0;
    int addMode_srcop,addMode_destop;

    addMode_srcop = getAddMode(src_op);
    addMode_destop = getAddMode(dest_op);

    for(i = 0;i<NUM_OF_CMDS;i++){
        if(strcmp(cmd,COMMANDS[i].cmd) == 0) {
            switch(addMode_srcop){
                case ADDMODE_DIRECT:
                    if(COMMANDS[i].addressingMode_op1.direct == OFF) return E_INVALID_SRCOP_ADDMODE;
                    break;
                case ADDMODE_IMMEDIATE:
                    if(COMMANDS[i].addressingMode_op1.immediate == OFF) return E_INVALID_SRCOP_ADDMODE;
                    break;
                case ADDMODE_MATRIX:
                    if(COMMANDS[i].addressingMode_op1.matrix == OFF) return E_INVALID_SRCOP_ADDMODE;
                    break;
                case ADDMODE_REG:
                    if(COMMANDS[i].addressingMode_op1.reg == OFF) return E_INVALID_SRCOP_ADDMODE;
                    break;
                case ADDMODE_NO_OPERAND:
                    if(COMMANDS[i].addressingMode_op1.noOperand == OFF) return E_INVALID_SRCOP_ADDMODE;
                    break;
                default:
                    return E_INVALID_SRCOP_ADDMODE;
            }

            switch(addMode_destop){
                case ADDMODE_DIRECT:
                    if (COMMANDS[i].addressingMode_op2.direct == OFF) return E_INVALID_DESTOP_ADDMODE;
                    break;
                case ADDMODE_IMMEDIATE:
                    if (COMMANDS[i].addressingMode_op2.immediate == OFF) return E_INVALID_DESTOP_ADDMODE;
                    break;
                case ADDMODE_MATRIX:
                    if (COMMANDS[i].addressingMode_op2.matrix == OFF) return E_INVALID_DESTOP_ADDMODE;
                    break;
                case ADDMODE_REG:
                    if (COMMANDS[i].addressingMode_op2.reg == OFF) return E_INVALID_DESTOP_ADDMODE;
                    break;
                case ADDMODE_NO_OPERAND:
                    if (COMMANDS[i].addressingMode_op2.noOperand == OFF) return E_INVALID_DESTOP_ADDMODE;
                    break;
                default:
                    return E_INVALID_DESTOP_ADDMODE;
            }
            return E_SUCCESS;
        }
    }
    return FALSE;
}

