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
                                         {"not",4,{1,0,0,0,0},{0,0,1,1,1}},
                                         {"clr",5,{1,0,0,0,0},{0,0,1,1,1}},
                                         {"lea",6,{0,0,1,1,0},{0,0,1,1,1}},
                                         {"inc",7,{1,0,0,0,0},{0,0,1,1,1}},
                                         {"dec",8,{1,0,0,0,0},{0,0,1,1,1}},
                                         {"jmp",9,{1,0,0,0,0},{0,0,1,1,1}},
                                         {"bne",10,{1,0,0,0,0},{0,0,1,1,1}},
                                         {"red",11,{1,0,0,0,0},{0,0,1,1,1}},
                                         {"prn",12,{1,0,0,0,0},{0,1,1,1,1}},
                                         {"jsr",13,{1,0,0,0,0},{0,0,1,1,1}},
                                         {"rts",14,{1,0,0,0,0},{1,0,0,0,0}},
                                         {"stop",15,{1,0,0,0,0},{1,0,0,0,0}}
                                        };


const char * const REGISTERS[NUM_OF_REG] = {"r0","r1","r2","r3","r4","r5","r6","r7"};


static symbolTable *symbolTab_tail = NULL, *symbolTab_head = NULL;
static dataCounter *data_counter = NULL;

static int dc = 0;
static int ic = 0;

static unsigned code[MAX_FILE_SIZE] = {1,0,0,0,0};
static unsigned data[MAX_FILE_SIZE] = {1,0,0,0,0};


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

err_t updateIc(char *cmd,char *dest_op,char *src_op,int status){

    err_t state;
    int word, i;
    if((state = isValidAddressMode(cmd,dest_op,src_op)) != E_SUCCESS)
        return state;

    for(i = 0;i < NUM_OF_CMDS;i++){
        if(strcmp(cmd,COMMANDS[i].cmd) == 0)
            code[ic] |= COMMANDS[i].code<<6;
    }
    /* TODO: */
    code[ic] |= getAddMode(src_op,VALUE)<<4;
    code[ic++] |= getAddMode(dest_op,VALUE)<<2;


    /* TODO: fix code duplication*/

    if(isLabel(dest_op)){
        if(status == FIRST_PASS)
            ic++;
        else{

        }
    }
    if(isNum(dest_op))
    {
        word = atoi(dest_op);
        if(word < 0)
            word = ~((-1)*word) + 1;
        code[ic++] = word;
    }
    else if(dest_op[0] == '#' && isNum(&dest_op[1])){
        word = atoi(&dest_op[1]);
        if(word < 0)
            word = ~((-1)*word) + 1;
        code[ic++] = word;
    }
    else if(isReg(dest_op)){
        word = atoi(&dest_op[1]);
        code[ic++] = word;
    }
    else {
        char label[MAX_LINE], arg1[MAX_LINE],arg2[MAX_LINE];
        char *str;
        int reg1,reg2;
        str = strchr(dest_op,'[');
        if(str != NULL){
            strncpy(label,dest_op,str-dest_op);
            if(!isLabel(label)|| !isValidMat(str))
                return E_INVALID_DEST_OP;
            ic++;       /* no label adress yet, progressing one step */
            cpyMatVals(str,arg1,arg2);
            reg1 = atoi(&arg1[1]);
            reg2 = atoi(&arg2[1]);
            code[ic] |= reg1<<6;
            code[ic++] |= reg2<<2;

        }
        else
            return E_INVALID_DEST_OP;
    }

    if(isLabel(src_op)){
        if(status == FIRST_PASS)
            ic++;
        else{

        }
    }
    if(isNum(src_op))
    {
        word = atoi(src_op);
        if(word < 0)
            word = ~((-1)*word) + 1;
        code[ic++] = word;
    }
    else if(src_op[0] == '#' && isNum(&src_op[1])){
        word = atoi(&src_op[1]);
        if(word < 0)
            word = ~((-1)*word) + 1;
        code[ic++] = word;
    }
    else if(isReg(src_op)){
        word = atoi(&src_op[1]);
        code[ic++] = word<<6;
    }
    else {
        char label[MAX_LINE], arg1[MAX_LINE],arg2[MAX_LINE];
        char *str;
        int reg1,reg2;
        str = strchr(src_op,'[');
        if(str != NULL){
            strncpy(label,src_op,str-src_op);
            if(!isLabel(label)|| !isValidMat(str))
                return E_INVALID_SRC_OP;
            ic++;       /* no label adress yet, progressing one step */
            cpyMatVals(str,arg1,arg2);
            reg1 = atoi(&arg1[1]);
            reg2 = atoi(&arg2[1]);
            code[ic] |= reg1<<6;
            code[ic++] |= reg2<<2;

        }
        else
            return E_INVALID_SRC_OP;
    }

    return E_SUCCESS;

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

void printCodeBits(){
    int i;
    unsigned mask =  1;
    printf("Code counter:\n");
    for(i = 0;i<=ic;i++){
        printf("\t%3d:",i);
        for(mask = 1,mask <<= 9;mask;mask>>= 1) {
            printf("%d", mask & code[i]? 1: 0);
            if(mask == 64 || mask == 4)
                printf("-");
        }
        printf("\n");

    }
    printf("\n");
}