//
// Created by naortif on 7/26/17.
//


#include "content_validation.h"
#include "data_struct.h"
#include "error_handler.h"
#include "content_validation.h"

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


static symbolTable *symbolTab_head = NULL,*symbolTab_tail = NULL;



static dataCounter *data_counter = NULL;

static int dc = STARTING_ADD;
static int ic = STARTING_ADD;
static int offset = 0;

static unsigned code[MAX_FILE_SIZE] = {0};
static unsigned data[MAX_FILE_SIZE] = {0};

void *safe_malloc(int nmemb,size_t size){
    void *ptr = malloc(size*nmemb);
    if(ptr == NULL) {    /* TODO need to add code - error printing */
        fprintf(stderr,"Could not allocate memory");
        exit(EXIT_FAILURE);
    }

    return ptr;
}

symbolTable *fetch_label_symtable(){

}


enum ErrorTypes updateSymbolTable(char *label,int address,int position,int format,int iscmd){
    symbolTable *node;
    node = NEW_SYMTABLE_NODE(label,address,position,format,iscmd)

    if(symbolTab_head == NULL) {
        symbolTab_tail = symbolTab_head = node;
        return NO_ERR_OCCURRED;
    }

    symbolTab_tail = symbolTab_head;

    LOOP {
        if (strcmp(symbolTab_tail->label, node->label) == 0)
            return ERR_LABEL_REDECLARED;
        else if (strcmp(symbolTab_tail->label, node->label) < 0) {
            if (symbolTab_tail->left == NULL) {
                symbolTab_tail->left = node;
                if(format)
                    dc++;
                return NO_ERR_OCCURRED;
            } else
                symbolTab_tail = symbolTab_tail->left;
        } else {
            if (symbolTab_tail->right == NULL) {
                symbolTab_tail->right = node;
                if(format)
                    dc++;
                return NO_ERR_OCCURRED;
            } else
                symbolTab_tail = symbolTab_tail->right;
        }
    }

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


symbolTable *get_symbolId(char *label){
    symbolTable *node = symbolTab_head;
    while(node){
        if((strcmp(node->label,label)) == 0)
            return node;
        else if(strcmp(node->label,label) < 0)
            node = node->left;
        else
            node = node->right;
    }
    return NULL;
}

int symbToBin(symbolTable *symb){
    return ((symb->address + (symb->iscmd == NOT_CMD2?offset:0))<<2) | symb->position;
}

void strToBinWord(char *str,AddressMode mode,int op_type,int passage){
    int bits = 0;
    static struct {
        unsigned is_srcop_reg:2;
        unsigned is_destop_reg:2;
    }flag = {FALSE,FALSE};
    if(op_type == SRC_OP) flag.is_srcop_reg = FALSE;
    symbolTable *symbolId;
    char label[MAX_LINE] = "";
    char arg1[MAX_LINE],arg2[MAX_LINE];

    switch(mode){
        case ADDMODE_IMMEDIATE:
            bits = atoi(&str[1]);
            if(bits < 0)
                bits = ~((-1)*bits) + 1;
            code[++ic] |= bits<<2;
            break;
        case ADDMODE_DIRECT:
            if(passage == SECOND_PASS && (symbolId = get_symbolId(str)) != NULL)
                code[++ic] |= symbToBin(get_symbolId(str));
            else
                code[++ic] |= 0;
            break;
        case ADDMODE_MATRIX:
            strncpy(label,str,strchr(str,'[')-str);
            if(passage == FIRST_PASS)
                ic++;       /* no label adress yet, progressing one step */
            else if(passage == SECOND_PASS){
                code[++ic] |= symbToBin(get_symbolId(label));
            }
            cpyMatVals(str,arg1,arg2);
            code[++ic] |= (atoi(&arg1[1])<<6) | atoi(&arg2[1])<<2;
            break;
        case ADDMODE_REG:
            if(op_type==SRC_OP)
                flag.is_srcop_reg = TRUE,ic++;
            if(op_type==DEST_OP && flag.is_srcop_reg != TRUE)
                ic++;
            bits = atoi(&str[1]);
            code[ic] |= bits<<(op_type == SRC_OP?6:2);
            break;
        case ADDMODE_NO_OPERAND:
            code[ic] |= 0;
        default:
            return ;
    }

}

void set_offset(void){
    offset = ic-STARTING_ADD;
}

err_t updateIc(char *cmd,char *src_op,char *dest_op,int passage){
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
    code[ic] |= (srcop_mode == ADDMODE_NO_OPERAND?0:srcop_mode)<<4;
    code[ic] |= (destop_mode == ADDMODE_NO_OPERAND?0:destop_mode)<<2;

    strToBinWord(src_op,srcop_mode,SRC_OP,passage);
    strToBinWord(dest_op,destop_mode,DEST_OP,passage);

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
    testfile = fopen(strcat(strcat(filename,".as.test."),pass),"r");
    if(testfile == NULL)
        exit(EXIT_FAILURE);
    if((strcmp(lvl,"code") == 0)|| strcmp(lvl,"complete") == 0){
    printf("\n****************************************************\n\nCode counter:\n");
    for(i = STARTING_ADD;i<ic;i++){
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
        for(i = STARTING_ADD;i<dc;i++){
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

void clear_data_stacks(void){
    while(dc > STARTING_ADD)
        data[dc--] = 0;
    while(ic > STARTING_ADD)
        data[ic--] = 0;
    data[dc] = 0;
    data[ic] = 0;
}

void free_symtree(symbolTable* node) {
    if (node == NULL)
        return;
    if (node->right == NULL && node->left == NULL)
        free(node);
    else {
        free_symtree(node->right);
        free_symtree(node->left);
    }
}

void free_symbtable(void){
    free_symtree(symbolTab_head);
    symbolTab_tail = symbolTab_head = NULL;
}

void create_ob_file(char *fileName){
    FILE *outf;
    if((outf = fopen(strcat(fileName,OUT_OB),"w")) == NULL)
    {
        fprintf(stderr,"Could not write to %s","%s");
        exit(EXIT_FAILURE);
    }

    fclose(outf);
}

void create_ext_file(char *fileName){
    FILE *outf;
    char outFileName[MAX_FILE_NAME];
    if((outf = fopen(strcat(fileName,OUT_EXT),"w")) == NULL)
    {
        fprintf(stderr,"Could not write to %s","%s");
        exit(EXIT_FAILURE);
    }

    fclose(outf);
}

void create_ent_file(char *fileName){
    FILE *outf;
    if((outf = fopen(strcat(fileName,OUT_ENT),"w")) == NULL)
    {
        fprintf(stderr,"Could not write to %s","%s");
        exit(EXIT_FAILURE);
    }

    fclose(outf);
}


