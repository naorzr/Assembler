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
                                         {"mov",0,"immediate/direct/matrix/direct_reg","direct/matrix/direct_reg"},
                                         {"cmp",1,"immediate/direct/matrix/direct_reg","immediate/direct/matrix/direct_reg"},
                                         {"add",2,"immediate/direct/matrix/direct_reg","direct/matrix/direct_reg"},
                                         {"sub",3,"immediate/direct/matrix/direct_reg","direct/matrix/direct_reg"},
                                         {"not",4,"null","direct/matrix/direct_reg"},
                                         {"clr",5,"null","direct/matrix/direct_reg"},
                                         {"lea",6,"direct/matrix","direct/matrix/direct_reg"},
                                         {"inc",7,"null","direct/matrix/direct_reg"},
                                         {"dec",8,"null","direct/matrix/direct_reg"},
                                         {"jmp",9,"null","direct/matrix/direct_reg"},
                                         {"bne",10,"null","direct/matrix/direct_reg"},
                                         {"red",11,"null","direct/matrix/direct_reg"},
                                         {"prn",12,"null","immediate/direct/matrix/direct_reg"},
                                         {"jsr",13,"null","direct/matrix/direct_reg"},
                                         {"rts",14,"null","null"},
                                         {"stop",15,"null","null"}
                                        };


const char * const REGISTERS[NUM_OF_REG] = {"r0","r1","r2","r3","r4","r5","r6","r7"};


static symbolTable *symbolTab_tail = NULL, *symbolTab_head = NULL;
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

void updateIc(char *op1,char *op2,int state){
    if(state == FIRST_PASS && (isLabel(op1) || isLabel(op2)))
    {
        ic++;
        return;
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