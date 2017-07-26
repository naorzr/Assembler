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

symbolTable *symlloc(void){
    symbolTable *ptr = (symbolTable *) malloc(sizeof(symbolTable));
    if(ptr)    /* need to add code - error printing */
        return ptr;
    exit(EXIT_FAILURE);

}
void updateSymbolTable(char *label,int address,int storageType,int iscmd);

void updateDc(char *op1,char *op2);

int getDc();

int getIc();

