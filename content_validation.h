//
// Created by naortif on 7/24/17.
//
#ifndef ENDPROJECT_CONTENT_VALIDATION_H
#define ENDPROJECT_CONTENT_VALIDATION_H

#include "string.h"

enum {OFF,ON};
enum {FALSE = 0,TRUE = 1};
typedef enum {
    ADDMODE_INVALID = -2,
    ADDMODE_NO_OPERAND = -1,
    ADDMODE_IMMEDIATE = 0,
    ADDMODE_DIRECT = 1,
    ADDMODE_MATRIX = 2,
    ADDMODE_REG = 3,
    }AddressMode;

#define Is_External(label) strcmp((label),"extern") == 0
#define Is_Entry(label)     strcmp((label),"entry") == 0
#define LABEL_DEC(word)    (word)[(strlen((word)))-1] == ':'
#define IS_DIRECTIVE(word)     (word)[0] == '.'

//typedef enum err_t{
//    E_SUCCESS = 1,
//    E_LABEL_REDEC,    /* label redeclaration */
//    E_INVALID_LABEL,
//    E_INVALID_SRC_OP,
//    E_INVALID_DEST_OP,
//    E_INVALID_SRCOP_ADDMODE,
//    E_INVALID_DESTOP_ADDMODE,
//
//}err_t;

int isReg(char *word);

int is_label(char *label);

int is_dsm(char *word);

int isCmd(char *word);

int isString(char *str);

int isNum(char *str);

int cpyMatVals(const char *mat,char *arg1,char *arg2);

int isValidMat(char *str);

int getAddMode(char *op);

enum ErrorTypes isValidAddressMode(char *cmd,AddressMode src_op, AddressMode dest_op);

int validMatInitializer(const char *mat);

#endif //ENDPROJECT_CONTENT_VALIDATION_H