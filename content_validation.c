#include <string.h>
#include <ctype.h>
#include "content_validation.h"
#include "assembler.h"
#include "data_struct.h"
#include "helpers.h"
#include "error_handler.h"

int is_label(char *label){
    int i,slen;
    if((slen = strlen(label)) > 30 || slen == 0  || !isalpha(label[0]) || is_dsm(label) || Is_External(label) || Is_Entry(label) || isCmd(label) || isReg(label))
        return FALSE;

    for(i = 0; i < slen-1 ;i++)
        if(!isalnum(label[i]))
            return FALSE;

    return TRUE;
}

int is_dsm(char *word){
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
    Skip_Space(str);
    if(*str == '-' || *str == '+')
        str++;

    while (isdigit(*str) != '\0')
            str++;

    if (*str == ' ')
        Skip_Space(str);

    return *str == '\0'?TRUE:FALSE;
}

int isReg(char *op){
    extern const char * const REGISTERS[NUM_OF_REG];
    int i;
    for(i = 0;i < NUM_OF_REG;i++)
        if(strcmp(REGISTERS[i],op) == 0)
            return TRUE;

    return FALSE;
}

int cpyMatVals(const char *mat, char *arg1, char *arg2) {
    char *op1, *op2;
    int i;
    char temp_mat[MAX_LINE] = "";
    strcpy(temp_mat, mat);
    /* TODO: fix that skip space with the temp_mat. function not working cause its an array being passed(allocate dynamic memory) */

    op1 = strchr(mat, '[');
    for (i = 0; i < MAX_LINE && *(++op1) != ']'; i++)
        if (isspace(*op1))
            continue;
        else
            arg1[i] = *op1;
    arg1[i] = '\0';

    op2 = strchr(op1, '[');
    for (i = 0; i < MAX_LINE && *(++op2) != ']'; i++)
        if (isspace(*op2))
            continue;
        else
            arg2[i] = *op2;
    arg2[i] = '\0';

    return TRUE;
}

int validMatInitializer(const char *mat){
    char arg1[MAX_LINE],arg2[MAX_LINE];
    if(!valid_parentheses(mat))
        return FALSE;
    cpyMatVals(mat,arg1,arg2);
    if(isNum(arg1) && isNum(arg2))
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

/**
 * Checks matrix arguments validity
 * @param str matrix string
 * @return
 */
int isValidMat(char *str) {
    char arg1[MAX_LINE] = "", arg2[MAX_LINE] = "";

    if (cpyMatVals(str, arg1, arg2) == TRUE)
        return (isNum(arg1) || isReg(arg1)) &&
               (isNum(arg2) || isReg(arg2));

}

int getAddMode(char *op) {
    if (strcmp(op, "") == 0)
        return ADDMODE_NO_OPERAND;
    else if (op[0] == '#')
        return ADDMODE_IMMEDIATE;
    else if (is_label(op))
        return ADDMODE_DIRECT;
    else if (isReg(op))
        return ADDMODE_REG;
    else {
        char label[MAX_LINE] = "";
        char *str;
        str = strchr(op, '[');
        strncpy(label, op, str - op);
        if (!is_label(label) || !isValidMat(str))
            return ADDMODE_INVALID;
        return ADDMODE_MATRIX;
    }
}


enum ErrorTypes isValidAddressMode(char *cmd, AddressMode src_op, AddressMode dest_op) {
    extern const struct COMMAND const COMMANDS[NUM_OF_CMDS];
    int i = 0;

    for (i = 0; i < NUM_OF_CMDS; i++) {
        if (strcmp(cmd, COMMANDS[i].cmd) == 0) {
            switch (src_op) {
                case ADDMODE_DIRECT:
                    if (COMMANDS[i].addressingMode_op1.direct == OFF) return E_INVALID_SRCOP_ADDMODE;
                    break;
                case ADDMODE_IMMEDIATE:
                    if (COMMANDS[i].addressingMode_op1.immediate == OFF) return E_INVALID_SRCOP_ADDMODE;
                    break;
                case ADDMODE_MATRIX:
                    if (COMMANDS[i].addressingMode_op1.matrix == OFF) return E_INVALID_SRCOP_ADDMODE;
                    break;
                case ADDMODE_REG:
                    if (COMMANDS[i].addressingMode_op1.reg == OFF) return E_INVALID_SRCOP_ADDMODE;
                    break;
                case ADDMODE_NO_OPERAND:
                    if (COMMANDS[i].addressingMode_op1.noOperand == OFF) return E_INVALID_SRCOP_ADDMODE;
                    break;
                default:
                    return E_INVALID_SRCOP_ADDMODE;
            }

            switch (dest_op) {
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
            return NO_ERR_OCCURRED;
        }
    }
    return FALSE;
}

