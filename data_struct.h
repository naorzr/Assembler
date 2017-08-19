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
#define WORD_LEN 5
#define ADDRESS_LEN 4

#define NEW_SYMTABLE_NODE(lab,add,position,format,iscmd)  (symbolTable *) safe_malloc(1,sizeof(symbolTable));\
                                                strcpy(node->label,(lab));\
                                                node->address = (add);\
                                                node->position = (position);\
                                                node->format = (format);\
                                                node->iscmd = (iscmd);\
                                                node-> right = node->left = NULL;\


enum {SRC_OP,DEST_OP};

enum positions{ABSOLUTE,EXTERNAL,RELOCATABLE};
enum{EXTERNAL_ADDRESS = 0, NONE,ENTRY=10,NONE_ENTRY,DSM,CMD2=4,NOT_CMD2=5,NO, YES,SAME_LINE,NEW_LINE,ADD_MODE,VALUE};

typedef struct symbolTable{
    char label[MAX_LINE];
    int address;
    int position;
    int format;
    int iscmd;
    struct symbolTable *left;
    struct symbolTable *right;
}symbolTable;

typedef struct dataCounter{
    unsigned memWord;
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

#define STARTING_ADD 100
#define OUT_EXT ".ext"
#define OUT_ENT ".ent"
#define OUT_OB ".ob"
#define AS_EXT ".as"

#define MAX_FILE_NAME 256

symbolTable *symlloc(void);

enum ErrorTypes updateSymbolTable(char *label,int address,int storageType,int isentry,int iscmd);

void updateData(char *directive,char *op2);

void updateIcCounter(char *op1,char *op2,int *ic);


err_t updateIc(char *cmd,char *op1,char *op2,int state);

void clear_data_stacks(void);

void free_symbtable(void);

unsigned numOfMemWords(char *operand,int state);

int getDc(void);

int getIc(void);

int exist_label(char *label);

void test(char *filename);

void set_offset(void);

void create_ob_file(char *outf);

void create_ent_file(char *outf);

void create_ext_file(char *outf);

#endif //ENDPROJECT_DATA_STRUCT_H