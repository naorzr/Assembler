//
// Created by naortif on 7/26/17.
//

#ifndef ENDPROJECT_FILE_CONTENT_H
#include "content_validation.h"
#endif

#ifndef ENDPROJECT_DATA_STRUCT_H
    #include "data_struct.h"
#endif


#include <string.h>
#include <stdio.h>
#include <ctype.h>

const struct COMMAND const COMMANDS[] = {
                                         {"mov",0,{0,1,1,1,1},{0,0,1,1,1}},
                                         {"cmp",1,{0,1,1,1,1},{0,1,1,1,1}},
                                         {"add",2,{0,1,1,1,1},{0,0,1,1,1}},
                                         {"sub",3,{0,1,1,1,1},{0,0,1,1,1}},
                                         {"not",4,{0},{0,0,1,1,1}},
                                         {"clr",5,{0},{0,0,1,1,1}},
                                         {"lea",6,{0,0,1,1,0},{0,0,1,1,1}},
                                         {"inc",7,{0},{0,0,1,1,1}},
                                         {"dec",8,{0},{0,0,1,1,1}},
                                         {"jmp",9,{0},{0,0,1,1,1}},
                                         {"bne",10,{0},{0,0,1,1,1}},
                                         {"red",11,{0},{0,0,1,1,1}},
                                         {"prn",12,{0},{0,1,1,1,1}},
                                         {"jsr",13,{0},{0,0,1,1,1}},
                                         {"rts",14,{0},{0}},
                                         {"stop",15,{0},{0}}
                                        };


const char * const REGISTERS[NUM_OF_REG] = {"r0","r1","r2","r3","r4","r5","r6","r7"};


static symbolTable *symbolTab_tail = NULL, *symbolTab_head = NULL;
static dataCounter *data_counter = NULL;

static int dc = 0;
static int ic = 0;

static unsigned code[MAX_FILE_SIZE];
static unsigned data[MAX_FILE_SIZE];


symbolTable *symlloc(void){
    symbolTable *ptr = (symbolTable *) malloc(sizeof(symbolTable));
    if(ptr)    /* TODO need to add code - error printing */
        return ptr;
    exit(EXIT_FAILURE);

}

void updateSymbolTable(char *label,int address,int storageType,int iscmd){
    if(symbolTab_head == NULL)
        symbolTab_head = symbolTab_tail = symlloc();
    else
        symbolTab_tail = (symbolTab_tail->next = symlloc());
    strcpy(symbolTab_tail->label,label);
    symbolTab_tail->address = address;
    symbolTab_tail->storageType = storageType;
    symbolTab_tail->iscmd = iscmd;

}

/* TODO finish this function */
/* numOfMemWords: takes the operand string and returns the number of words needed for it be stored */
unsigned numOfMemWords(char *operand,int state){
    if(isNum(operand) || isValidMat(operand) || isLabel(operand))
        return 1;

    if(isString(operand))
        return (unsigned) strlen(operand) - 1;  /* 1 extra word for the terminating null and reducing 2 word numbers for the quotation */





}

/* TODO needs to finish this function */
void updateDc(char *directive,char *op2){
    /*
    if(data_counter == NULL){
        data_counter = (dataCounter *) malloc(sizeof(dataCounter));
        if(data_counter == NULL){
            fprintf(stderr,"Could not allocate memory");
            exit(EXIT_FAILURE);
        }

    }
     */
}

void updateIcCounter(char *op1,char *op2,int *ic){

}

err_t updateIc(char *cmd,char *op1,char *op2,int status){
    err_t state;
    int word;
    if(status == FIRST_PASS && (isLabel(op1) || isLabel(op2)))
    {
        ic++;
    } else {
        if((state = isValidAddressMode(cmd,op1,op2)) != E_SUCCESS)
            return state;
        if(isNum(op1))
        {
            word = atoi(op1);
            if(word < 0)
                word = ~((-1)*word) + 1;
            code[ic++] = word;
        }
        else if(op1[0] == '#' && isNum(&op1[1])){
            word = atoi(&op1[1]);
            if(word < 0)
                word = ~((-1)*word) + 1;
            code[ic++] = word;
        }
        else if(isReg(word)){
            word = atoi(&op1[1]);
            code[ic++] = word;
        }
        else if(isValidMatVal(op1)){
            char label[MAX_LINE];
            char *str;
            str = strchr(op1,'[');
            if(str != NULL){
                strncpy(label,op1,str-op1-1);
                if(isLabel(label) == FALSE || isValidMat(str) == FALSE)
                    return E_INVALID_SRC_OP;
                ic++;       /* no label adress yet, progressing one step */

            }
        }

        return E_INVALID_SRC_OP;

    }






}

int getIc(void){
    return ic;
}

int getDc(void){
    return dc;
}

int exist_label(char *label){
    symbolTable *ptr = symbolTab_head;
    while(ptr)
        if(strcmp(ptr++->label,label) == 0)
            return TRUE;
    return FALSE;
}