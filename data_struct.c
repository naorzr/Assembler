//
// Created by naortif on 7/26/17.
//


#include "content_validation.h"
#include "data_struct.h"



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


/* TODO needs to finish this function */
void updateData(char *directive,char *op_string){
    char arg1[MAX_LINE],arg2[MAX_LINE],mat[MAX_LINE];
    char *param;
    int bitword, mat_word_size = 0;
    if(strcmp(directive,"data") == 0){
        param = strtok(op_string,",NULL");

        do {
            if (param == NULL || !isNum(param)) {
                printf("Invalid data param");
                return;
            }
            bitword = atoi(param);
            if (bitword < 0)
                bitword = ~(bitword * -1) + 1;
            data[dc++] = bitword;
        }while (param = strtok(NULL,",NULL"));

    } else if(strcmp(directive,"string") == 0){
        param = strtok(op_string,"");
        if(param == NULL || !isString(param)){
            printf("Invalid string param");
            return;
        }
        param[strlen(param)-1] = '\0';
        param = &param[1];
        while(*param)
            data[dc++] = *(param++);

        data[dc++] = '\0';

    } else if(strcmp(directive,"mat") == 0){
        param = strtok(op_string," ");
        if(!validMatInitializer(param)){
            printf("Invalid mat initializer");
            return;
        }
        cpyMatVals(param,arg1,arg2);
        mat_word_size = atoi(arg1)*atoi(arg2);

        while (param = strtok(NULL,",NULL")){
            mat_word_size--;
            if (!isNum(param)) {
                printf("Invalid data param");
                return;
            }
            bitword = atoi(param);
            if (bitword < 0)
                bitword = ~(bitword * -1) + 1;
            data[dc++] = bitword;
        }
        while(mat_word_size--)
            dc++;
    }
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

void test(const char *lvl,char *filename,char *pass){
    int i,k;
    unsigned mask =  1;
    FILE *testfile;
    char line[MAX_LINE];
    struct {
        unsigned tf: 2;
    }flag = {TRUE};
    testfile = fopen(strcat(strcat(filename,".test."),pass),"r");
    if(testfile == NULL)
        exit(EXIT_FAILURE);
    if((strcmp(lvl,"code") == 0)|| strcmp(lvl,"complete") == 0){
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
    }
    if((strcmp(lvl,"data") == 0) || strcmp(lvl,"complete") == 0) {
        printf("\n****************************************************\n\nData counter:\n");
        for(i = 0;i<dc;i++){
            flag.tf = TRUE;
            fgets(line,MAX_LINE,testfile);
            printf("%3d: ",i+ic);
            for(mask = 1,k = 0,mask <<= 9;mask;mask>>= 1,k++) {
                if(line[k] == '-')
                    k++;
                if((mask&data[i]?1:0) != (line[k] - '0'))
                    flag.tf = FALSE;
                printf("%d", mask & data[i]? 1: 0);
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
    }
    printf("\n");
}