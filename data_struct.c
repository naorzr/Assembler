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


const struct COMMAND const COMMANDS[] = {{"mov",0},{"cmp",1},{"add",2},{"sub",3},{"not",4},{"clr",5},
                                         {"lea",6},{"inc",7},{"dec",8},{"jmp",9},{"bne",10},{"red",11},
                                         {"prn",12},{"jsr",13},{"rts",14},{"stop",15}};

const char * const REGISTERS[NUM_OF_REG] = {"r0","r1","r2","r3","r4","r5","r6","r7"};


static symbolTable *tail = NULL, *head = NULL;
static dataCounter *data_counter = NULL;

static int dc = 0;
static int ic = 0;



symbolTable *symlloc(void){
    symbolTable *ptr = (symbolTable *) malloc(sizeof(symbolTable));
    if(ptr)    /* TODO need to add code - error printing */
        return ptr;
    exit(EXIT_FAILURE);

}

void updateSymbolTable(char *label,int address,int storageType,int iscmd){
    if(head == NULL)
        head = tail = symlloc();
    else
        tail = (tail->next = symlloc());
    strcpy(tail->label,label);
    tail->address = address;
    tail->storageType = storageType;
    tail->iscmd = iscmd;

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

int getIc(){
    return ic;
}

int getDc(){
    return dc;
}