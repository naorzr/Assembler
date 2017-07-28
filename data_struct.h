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
#define NUM_OF_CMDS 16


enum{OFF,ON};
enum{EXTERNAL_ADDRESS = -1, NONE,EXTERN,ENTRY,DSM,CMD,NOT_CMD,NO, YES};

typedef struct symbolTable{
    char label[MAX_LINE];
    int address;
    int storageType;
    int iscmd;
    struct symbolTable *next;
}symbolTable;

struct COMMAND{
    char *cmd;
    int code;
};


symbolTable *symlloc(void);

void updateSymbolTable(char *label,int address,int storageType,int iscmd);

void updateDc(char *op1,char *op2);

void updateIcCounter(char *op1,char *op2,int *ic);

int getDc();

int getIc();

