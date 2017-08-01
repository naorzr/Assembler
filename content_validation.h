//
// Created by naortif on 7/24/17.
//
#ifndef ENDPROJECT_CONTENT_VALIDATION_H
#define ENDPROJECT_CONTENT_VALIDATION_H

#endif //ENDPROJECT_CONTENT_VALIDATION_H

enum {FALSE = -1,TRUE};

#define IS_EXTERNAL(label) !strcmp((label),"extern") || !strcmp((label),"entry")
void printerr(char *lineContent,char *str,int lineNumber);

int validLabel(char *label);

int isDsm(char *word);

int isCmd(char *word);

int isastring(char *str);

int isanum(char *str);

int validmatvalue(char *val);

int getmatarg(char *mat,char *arg1,char *arg2);

int isamat(char *str);