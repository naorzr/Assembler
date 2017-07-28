//
// Created by naortif on 7/26/17.
//
#ifndef ENDPROJECT_DATA_STRUCT_H
    #include "data_struct.h"
#endif
#include <string.h>
#include <stdio.h>

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
    tail->label = (char *) malloc(sizeof(label) * (strlen(label)+1));
    if(!tail->label) {
        fprintf(stderr,"Could'nt allocate additional memory");
        exit(EXIT_FAILURE);
    }
    strcpy(tail->label,label);
    tail->address = address;
    tail->storageType = storageType;
    tail->iscmd = iscmd;
    tail = (tail->next = symlloc());
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