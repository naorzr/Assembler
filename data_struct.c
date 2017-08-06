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

static unsigned code[MAX_FILE_SIZE] = {0};
static unsigned data[MAX_FILE_SIZE] = {0};


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


int strToBinWord(char *str,AddressMode mode,int op_type){
    int word;
    char arg1[MAX_LINE],arg2[MAX_LINE];

    switch(mode){
        case ADDMODE_IMMEDIATE:
            word = atoi(&str[1]);
            if(word < 0)
                word = ~((-1)*word) + 1;
            return word<<2;
        case ADDMODE_DIRECT:
            word = atoi(str);
            if(word < 0)
                word = ~((-1)*word) + 1;
            return word<<2;
        case ADDMODE_MATRIX:
            str = strchr(str,'[');
            ic++;       /* no label adress yet, progressing one step */
            cpyMatVals(str,arg1,arg2);
            return (atoi(&arg1[1])<<6) | atoi(&arg2[1])<<2;
        case ADDMODE_REG:
            word = atoi(&str[1]);
            return word<<(op_type == SRC_OP?6:2);
        case ADDMODE_NO_OPERAND:
            return 0;
        default:
            return 0;
    }

}

err_t updateIc(char *cmd,char *src_op,char *dest_op,int status){
    err_t state;
    int word, i;
         /* will be the matrix argument if needed */
    char *str;
    AddressMode srcop_mode,destop_mode;

    destop_mode = getAddMode(dest_op);
    srcop_mode = getAddMode(src_op);

    if((state = isValidAddressMode(cmd,srcop_mode,destop_mode)) != E_SUCCESS)
        return state;

    for(i = 0;i < NUM_OF_CMDS;i++){
        if(strcmp(cmd,COMMANDS[i].cmd) == 0) {
            code[ic] |= COMMANDS[i].code << 6;
            break;
        }
    }

    /* TODO: */
    code[ic] |= srcop_mode == ADDMODE_NO_OPERAND?0:srcop_mode<<4;
    code[ic] |= destop_mode == ADDMODE_NO_OPERAND?0:destop_mode<<2;

    if(srcop_mode != ADDMODE_NO_OPERAND)
        code[++ic] |= strToBinWord(src_op,srcop_mode,SRC_OP);
    if(destop_mode != ADDMODE_NO_OPERAND)
        code[srcop_mode == ADDMODE_REG && destop_mode == ADDMODE_REG?ic:++ic] |= strToBinWord(dest_op,destop_mode,DEST_OP);

    ic++;
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

void test(char *lvl,char *filename){
    int i,k;
    unsigned mask =  1;
    FILE *testfile;
    char line[MAX_LINE];
    struct {
        unsigned tf: 2;
    }flag = {TRUE};
    testfile = fopen(strcat(strcat(filename,".test."),lvl),"r");

    printf("\n****************************************************\n\nCode counter:\n");
    for(i = 0;i<ic;i++){
        flag.tf = TRUE;
        fgets(line,MAX_LINE,testfile);
        printf("%3d: ",i);
        for(mask = 1,k = 0,mask <<= 9;mask;mask>>= 1,k++) {
            if(line[k] == '-')
                k++;
            if((mask&code[i]?1:0) != (line[k] - '0'))
                flag.tf = FALSE;
            printf("%d", mask & code[i]? 1: 0);
            if(mask == 64 || mask == 4)
                printf("-");
        }
        if(strchr(line,'\n'))
            line[strchr(line,'\n')-line] = '\0';

        printf("  %s",line);
        if(flag.tf == FALSE)
            printf("\t\t ** No Match **");
        else
            printf("\t\t **  Match   **");
        printf("\n");

    }
    printf("\n");
}