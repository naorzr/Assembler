#include <string.h>
#include <ctype.h>
#include "content_validation.h"
#include "helpers.h"

int is_label(char *label){
    int i;
    unsigned slen;
    if((slen = strlen(label)) > 30 || slen == 0  || !isalpha(label[0]) || is_dsm(label) || Is_External(label) || Is_Entry(label) || is_cmd(label) || is_reg(label))
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

int is_cmd(char *word){
    int i = 0;
    extern const struct Command COMMANDS[NUM_OF_CMDS];
    for(i = 0;i<NUM_OF_CMDS;i++)
        if(strcmp(COMMANDS[i].cmd,word) == 0)
            return TRUE;

    return FALSE;
}


int is_string(char *str){
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
int valid_pos_num(char *str) {
    if (is_num(str)) {
        int num = atoi(str);
        if (num >= MIN_POS_INT && num < MAX_POS_INT) {
            return TRUE;
        }
    }

    return FALSE;
}

int is_num(char *str){
    Skip_Space(str);
    if(*str == '-' || *str == '+')
        str++;

    while (isdigit(*str) != '\0')
        str++;

    if (*str == ' ')
        Skip_Space(str);

    return *str == '\0'?TRUE:FALSE;
}

int is_reg(char *op){
    extern const char *const REGISTERS[NUM_OF_REG];
    int i;
    for(i = 0;i < NUM_OF_REG;i++)
        if(strcmp(REGISTERS[i],op) == 0)
            return TRUE;

    return FALSE;
}

int valid_parentheses(char *str){
    char *left,*right;
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


/* valid mat initializer */
int valid_mat_init(char *mat){
    char arg1[MAX_LINE],arg2[MAX_LINE];
    if(!valid_parentheses(mat))
        return FALSE;
    cpy_mat_vals(mat,arg1,arg2);
    if(valid_pos_num(arg1) && valid_pos_num(arg2))
        return TRUE;
    return FALSE;
}

/**
 * Checks matrix arguments validity
 * @param str matrix string
 * @return
 */
int is_mat(char *str) {
    char arg1[MAX_LINE] = "", arg2[MAX_LINE] = "";

    if (valid_parentheses(str))
        cpy_mat_vals(str, arg1, arg2);

    return (valid_pos_num(arg1) || is_reg(arg1)) &&
           (valid_pos_num(arg2) || is_reg(arg2));


}

/**
 * Checks if operand is immediate (number which starts with #)
 * e.g. #1, #-1
 * @param op Operand string
 * @return
 */
int is_immediate(char *op) {
    int num = atoi(op);
    if (is_num(op) && valid_num_val(num)) {
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
int valid_commas(char *str) {
    unsigned len = strlen(str);
    int     i,
            commaCount = 0,
            inString = FALSE;

    for(i=0; i < len; i++) {
        if (str[i] == '"')
            inString = !inString;
        if (inString == TRUE)
            continue;
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


/**
 * Checks if operand addressing mode is valid with a given command
 * @param op Operand addressing mode
 * @param cmdAddMode command addressing mode rules
 * @return error code
 */
ErrorTypes isOpAddressModeValid(AddressModeType op, struct addressingMode cmdAddMode) {
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
ErrorTypes valid_address_mode(char *cmd, AddressModeType src_op, AddressModeType dest_op) {
    extern const struct Command COMMANDS[NUM_OF_CMDS];
    int i = 0;
    for (i = 0; i < NUM_OF_CMDS; i++) {
        if (strcmp(cmd, COMMANDS[i].cmd) == 0) {
            ErrorTypes res;
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


/**
 * Checks if a number is valid (between 511 and -511)
 * @param num
 * @return
 */
int valid_num_val(int num) {
    return num < MAX_POS_INT && num > MAX_NEG_INT;
}

