//
// Created by naortif on 7/24/17.
//
#ifndef ENDPROJECT_CONTENT_VALIDATION_H
#define ENDPROJECT_CONTENT_VALIDATION_H

#endif //ENDPROJECT_CONTENT_VALIDATION_H

enum {FALSE = -1,TRUE = 1};

#define Is_External(label) !strcmp((label),"extern")
#define Is_Entry(label)     !strcmp((label),"entry")

typedef enum err_t{
    E_SUCCESS,
    E_LABEL_REDEC,    /* label redeclaration */
    E_INVALID_LABEL,
    E_INVALID_SRC_OP,
    E_INVALID_DEST_OP,

}err_t;
void printerr(char *lineContent,char *str,int lineNumber);

int isLabel(char *label);

int isDsm(char *word);

int isCmd(char *word);

int isString(char *str);

int isNum(char *str);

int isValidMatVal(char *val);

int cpyMatVals(char *mat,char *arg1,char *arg2);

int isValidMat(char *str);

int isValidAddressMode(char *cmd,char *src_op, char *dest_op);