//
// Created by naortif on 7/24/17.
//
#ifndef ENDPROJECT_CONTENT_VALIDATION_H
#define ENDPROJECT_CONTENT_VALIDATION_H

#endif //ENDPROJECT_CONTENT_VALIDATION_H

enum {FALSE,TRUE};

#define IS_EXTERNAL(label) !strcmp((label),"extern") || !strcmp((label),"entry")
void printerr(char *lineContent,char *str,int lineNumber);

int validLabel(char *label);

int isDsm(char *word);

int isCmd(char *word);