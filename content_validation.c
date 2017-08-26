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

/**
 * Checks if a num is positive and valid (0-511)
 * @param str number as string
 * @return true if number is valid
 */
int isValidPositiveNum(char *str) {
    if (isNum(str)) {
        int num = atoi(str);
        if (num >= 0 && num < 512) {
            return TRUE;
        }
    }

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
    if(isValidPositiveNum(arg1) && isValidPositiveNum(arg2))
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

    if (valid_parentheses(str) && cpyMatVals(str, arg1, arg2) == TRUE)
        return (isValidPositiveNum(arg1) || isReg(arg1)) &&
               (isValidPositiveNum(arg2) || isReg(arg2));

}

/**
 * Checks if operand is immediate (number which starts with #)
 * e.g. #1, #-1
 * @param op Operand string
 * @return
 */
int is_immediate(char *op) {
    int num = atoi(op);
    if (isNum(op) && isValidNumVal(num)) {
        return TRUE;
    }
    return FALSE;
}

/**
 * Validates commas validity
 * e.g. cmp r1,,r2 or .mat [2][2] 1,2,3,4, are illigal
 * @param str
 * @return
 */
int validateCommas(char *str) {
    int i,
        len = strlen(str),
        commaCount = 0;

    for(i=0; i < len; i++) {
        if (isspace(str[i]))
            continue;
        if (str[i] == ',')
            commaCount++;
        else
            commaCount = 0;
        /* comma cannot come after another comma */
        if (commaCount > 1)
            return FALSE;
    }
    /* line cannot end with a comma */
    return commaCount > 0 ? FALSE: TRUE;

}

int getAddMode(char *op) {
    if (strcmp(op, "") == 0)
        return ADDMODE_NO_OPERAND;
    else if (op[0] == '#')
        return is_immediate(op + 1) == TRUE ? ADDMODE_IMMEDIATE : ADDMODE_INVALID;
    else if (is_label(op))
        return ADDMODE_DIRECT;
    else if (isReg(op))
        return ADDMODE_REG;
    else {
        char label[MAX_LINE] = "";
        char *str;
        str = strchr(op, '[');
        if (str == NULL)
            return ADDMODE_INVALID;
        strncpy(label, op, str - op);
        if (!is_label(label) || !isValidMat(str))
            return ADDMODE_INVALID;
        return ADDMODE_MATRIX;
    }
}

/**
 * Checks if operand addressing mode is valid with a given command
 * @param op Operand adressing mode
 * @param cmdAddMode command addressing mode rules
 * @return error code
 */
enum ErrorTypes isOpAddressModeValid(AddressModeType op, struct addressingMode cmdAddMode) {
    int errFlag = FALSE;
    switch (op) {
        case ADDMODE_DIRECT:
            if (cmdAddMode.direct == OFF) errFlag = TRUE;
            break;
        case ADDMODE_IMMEDIATE:
            if (cmdAddMode.immediate == OFF) errFlag = TRUE;
            break;
        case ADDMODE_MATRIX:
            if (cmdAddMode.matrix == OFF) errFlag = TRUE;
            break;
        case ADDMODE_REG:
            if (cmdAddMode.reg == OFF) errFlag = TRUE;
            break;
        case ADDMODE_NO_OPERAND:
            if (cmdAddMode.noOperand == OFF) return E_MISS_OP;
            break;
        default:
            errFlag = TRUE;
    }

    return errFlag ? E_INVALID_ADDMODE: NO_ERR_OCCURRED;
}

/**
 * Validate if the addressing modes are correct
 * @param cmd command
 * @param src_op source operand addressing mode
 * @param dest_op destination operand addressing mode
 * @return error code
 */
enum ErrorTypes isValidAddressMode(char *cmd, AddressModeType src_op, AddressModeType dest_op) {
    extern const struct COMMAND const COMMANDS[NUM_OF_CMDS];
    int i = 0;
    for (i = 0; i < NUM_OF_CMDS; i++) {
        if (strcmp(cmd, COMMANDS[i].cmd) == 0) {
            enum ErrorTypes res;
            /*  */
            res = isOpAddressModeValid(src_op, COMMANDS[i].addressingMode_op1);
            if (res != NO_ERR_OCCURRED)
                return res != E_INVALID_ADDMODE ? res : E_INVALID_SRCOP_ADDMODE;

            res = isOpAddressModeValid(dest_op, COMMANDS[i].addressingMode_op2);
            if (res != NO_ERR_OCCURRED)
                return res != E_INVALID_ADDMODE ? res : E_INVALID_DESTOP_ADDMODE;
        }
    }
    return NO_ERR_OCCURRED;
}

