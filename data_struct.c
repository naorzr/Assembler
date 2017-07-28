//
// Created by naortif on 7/26/17.
//
#ifndef ENDPROJECT_DATA_STRUCT_H
    #include "data_struct.h"
#endif
#include <string.h>
#include <stdio.h>


extern const struct COMMAND const COMMANDS[NUM_OF_CMDS];

const struct COMMAND const COMMANDS[] = {{"mov",0},{"cmp",1},{"add",2},{"sub",3},{"not",4},{"clr",5},
                                         {"lea",6},{"inc",7},{"dec",8},{"jmp",9},{"bne",10},{"red",11},
                                         {"prn",12},{"jsr",13},{"rts",14},{"stop",15}};

static symbolTable *tail = NULL, *head = NULL;

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

void updateDc(char *op1,char *op2){

}

void updateIcCounter(char *op1,char *op2,int *ic){

}

int getIc(){
    return ic;
}

int getDc(){
    return dc;
}