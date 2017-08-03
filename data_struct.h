//
// Created by naortif on 7/26/17.
//

#include <stdlib.h>
#ifndef ENDPROJECT_FILE_CONTENT_H
#include "file_content.h"
#endif
#ifndef ENDPROJECT_DATA_STRUCT_H
#define ENDPROJECT_DATA_STRUCT_H

#endif //ENDPROJECT_DATA_STRUCT_H

#define NUM_OF_REG 8
#define NUM_OF_CMDS 16
#define MAX_OPERANDS 40
#define MAX_FILE_SIZE 256

enum{OFF,ON};
enum{EXTERNAL_ADDRESS = -1, NONE,EXTERN,ENTRY,DSM,CMD,NOT_CMD,NO, YES,SAME_LINE,NEW_LINE};

typedef struct symbolTable{
    char label[MAX_LINE];
    int address;
    int storageType;
    int iscmd;
    struct symbolTable *next;
}symbolTable;

typedef struct dataCounter{
    unsigned memWord: 10;
}dataCounter;

const struct COMMAND{
    char *cmd;
    int code;
    const char *addressingMode_op1;
    const char *addressingMode_op2;
};




symbolTable *symlloc(void);

void updateSymbolTable(char *label,int address,int storageType,int iscmd);

void updateDc(char *directive,char *op2);

void updateIcCounter(char *op1,char *op2,int *ic);

void updateIc(char *op1,char *op2,int state);

unsigned numOfMemWords(char *operand,int state);

int getDc(void);

int getIc(void);

int exist_label(char *label);