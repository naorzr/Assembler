//
// Created by naortif on 7/26/17.
//

#include <stdlib.h>
#ifndef ENDPROJECT_DATA_STRUCT_H
#define ENDPROJECT_DATA_STRUCT_H
#include "assembler.h"
#include "content_validation.h"
#define LOOP  for(;;)
#define NUM_OF_REG 8
#define NUM_OF_CMDS 16
#define MAX_OPERANDS 40
#define MAX_FILE_SIZE 256

#define NEW_SYMTABLE_NODE(lab,add,storage,cmd)  (symbolTable *) safe_malloc(1,sizeof(symbolTable));\
                                                strcpy(node->label,(lab));\
                                                node->address = (add);\
                                                node->storageType = (storage);\
                                                node->iscmd = (cmd);\
                                                node-> right = node->left = NULL;\


enum {SRC_OP,DEST_OP};
enum{EXTERNAL_ADDRESS = -1, NONE,EXTERN,ENTRY,DSM,CMD,NOT_CMD,NO, YES,SAME_LINE,NEW_LINE,ADD_MODE,VALUE};

typedef struct symbolTable{
    char label[MAX_LINE];
    int address;
    int storageType;
    int iscmd;
    struct symbolTable *left;
    struct symbolTable *right;
}symbolTable;

typedef struct dataCounter{
    unsigned memWord: 10;
}dataCounter;

struct COMMAND{
    char *cmd;
    int code;
    struct{
     int noOperand: 2;
     int immediate: 2;
     int direct: 2;
     int matrix: 2;
     int reg: 2;
    }addressingMode_op1;
    struct{
        int noOperand: 2;
        int immediate: 2;
        int direct: 2;
        int matrix: 2;
        int reg: 2;
    } addressingMode_op2;
};




symbolTable *symlloc(void);

enum ErrorTypes updateSymbolTable(char *label,int address,int storageType,int iscmd);

void updateData(char *directive,char *op2);

void updateIcCounter(char *op1,char *op2,int *ic);


err_t updateIc(char *cmd,char *op1,char *op2,int state);

void clear_data_stacks(void);

void free_symbtable(void);

unsigned numOfMemWords(char *operand,int state);

int getDc(void);

int getIc(void);

int exist_label(char *label);

void test(const char *lvl,char *filename,char *pass);

#endif //ENDPROJECT_DATA_STRUCT_H