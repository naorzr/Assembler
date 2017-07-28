//
// Created by naortif on 7/26/17.
//

#include <stdlib.h>

#ifndef ENDPROJECT_DATA_STRUCT_H
#define ENDPROJECT_DATA_STRUCT_H

#endif //ENDPROJECT_DATA_STRUCT_H


typedef struct symbolTable{
    char *label;
    int address;
    int storageType;
    int iscmd;
    struct symbolTable *next;
}symbolTable;

const struct{
    char *cmd;
    int code;
}COMMANDS[] = {{"mov",0},{"cmp",1},{"add",2},{"sub",3},{"not",4},{"clr",5},
        {"lea",6},{"inc",7},{"dec",8},{"jmp",9},{"bne",10},{"red",11},
        {"prn",12},{"jsr",13},{"rts",14},{"stop",15},{"NULL",-1}};

symbolTable *symlloc(void);

void updateSymbolTable(char *label,int address,int storageType,int iscmd);

void updateDc(char *op1,char *op2);

void updateIcCounter(char *op1,char *op2,int *ic);

int getDc();

int getIc();

