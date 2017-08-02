//
// Created by naortif on 7/24/17.
//
#ifndef ENDPROJECT_CONTENT_VALIDATION_H
#define ENDPROJECT_CONTENT_VALIDATION_H

#endif //ENDPROJECT_CONTENT_VALIDATION_H

enum {FALSE = -1,TRUE = 1};

#define IS_EXTERNAL(label) !strcmp((label),"extern") || !strcmp((label),"entry")
void printerr(char *lineContent,char *str,int lineNumber);

int isLabel(char *label);

int isDsm(char *word);

int isCmd(char *word);

int isString(char *str);

int isNum(char *str);

int isValidMatVal(char *val);

int cpyMatVals(char *mat,char *arg1,char *arg2);

int isValidMat(char *str);