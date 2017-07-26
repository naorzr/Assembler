//
// Created by naortif on 7/26/17.
//

#include "data_struct.h"
#include <string.h>
#include <stdio.h>

static symbolTable *tail = NULL, *head = NULL;

static dc = 0; ic = 0;

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

int getIc(){
    return ic;
}

int getDc(){
    return dc;
}