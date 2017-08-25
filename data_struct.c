//
// Created by naortif on 7/26/17.
//


#include <string.h>
#include <stdio.h>


#include "content_validation.h"
#include "data_struct.h"
#include "error_handler.h"
#include "helpers.h"
#include "error_handler.h"
#include "logger.h"

/* Initialize the command with the appropriate addressing modes rules */
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


static int dc = STARTING_ADD;
static int ic = STARTING_ADD;
static int extref_ind = 0;
static int offset = 0;

static unsigned code[MAX_FILE_SIZE] = {0};
static unsigned data[MAX_FILE_SIZE] = {0};
static struct{
    const char * label;
    unsigned int address;
} extref[MAX_FILE_SIZE] = {0};

/**
 * Frees the external reference
 */
void freeExtRef() {
    extref_ind = 0;
    memset(&extref, 0, sizeof(extref));
}

/**
 * Clears the code array
 */
void clear_code_arr() {
    memset(code, 0, sizeof(code));
}

enum ErrorTypes updateSymbolTable(char *label, int address, int position, int format, int iscmd) {
    symbolTable *node;
    node = NEW_SYMTABLE_NODE(label, address, position, format, iscmd)

    if (symbolTab_head == NULL) {
        symbolTab_tail = symbolTab_head = node;
        return NO_ERR_OCCURRED;
    }

    symbolTab_tail = symbolTab_head;

    LOOP {
        if (strcmp(symbolTab_tail->label, node->label) == 0) {
            if(format == ENTRY) {
                symbolTab_tail->format = ENTRY;
                symbolTab_tail->position |= position;
                return NO_ERR_OCCURRED;
            }
            else
                return ERR_LABEL_REDECLARED;
        }
        else if (strcmp(symbolTab_tail->label, node->label) < 0) {
            if (symbolTab_tail->left == NULL) {
                symbolTab_tail->left = node;
                return NO_ERR_OCCURRED;
            } else
                symbolTab_tail = symbolTab_tail->left;
        } else {
            if (symbolTab_tail->right == NULL) {
                symbolTab_tail->right = node;
                return NO_ERR_OCCURRED;
            } else
                symbolTab_tail = symbolTab_tail->right;
        }
    }


}
/**
 * Checks if a number is valid (between 511 and -511)
 * @param num
 * @return
 */
int isValidNumVal(int num) {
    return num < 512 && num > -512;
}

/* TODO needs to finish this function */
enum ErrorTypes updateData(char *directive,char *op_string){

    char arg1[MAX_LINE],arg2[MAX_LINE];
    char *param;
    int bitword, mat_word_size = 0;
    if(strcmp(directive,"data") == 0){
        param = strtok(op_string,",NULL");

        do {
            if (param == NULL || !isNum(param)) {
                return ERR_INV_DATA_PARAM;
            }
            bitword = atoi(param);
            if (!isValidNumVal(bitword)) {
                return ERR_INV_DATA_SIZE;
            }
            if (bitword < 0)
                bitword = ~(bitword * -1) + 1;
            data[dc++] = bitword;
        } while (param = strtok(NULL,",NULL"));

    } else if(strcmp(directive,"string") == 0){
        param = strtok(op_string,"");
        if(param == NULL || !isString(param)){
            return ERR_INV_STRING_PARAM;
        }
        param[strlen(param)-1] = '\0';
        param = &param[1];
        while(*param)
            data[dc++] = *(param++);

        data[dc++] = '\0';

    } else if(strcmp(directive,"mat") == 0){
        param = strtok(op_string," ");
        if(!validMatInitializer(param)){
            return ERR_INV_MAT_INIT;
        }
        cpyMatVals(param,arg1,arg2);
        mat_word_size = atoi(arg1)*atoi(arg2);

        while (param = strtok(NULL,",NULL")){
            if(mat_word_size <= 0)
                return ERR_EXSS_MAT_VAL;
            mat_word_size--;
            if (!isNum(param)) {
                return ERR_INV_MAT_PARAM;
            }
            bitword = atoi(param);
            if (!isValidNumVal(bitword)) {
                return ERR_INV_MAT_PARAM_SIZE;
            }
            if (bitword < 0)
                bitword = ~(bitword * -1) + 1;
            data[dc++] = bitword;
        }
        while(mat_word_size--)
            dc++;
    }
    return NO_ERR_OCCURRED;
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
    if(symb->position == EXTERNAL) {
        extref[extref_ind].address = ic+1;
        extref[extref_ind].label = symb->label;
        extref_ind++;
    }
    return ((symb->address + (symb->iscmd == NOT_CMD2 && symb->position == RELOCATABLE?offset:0))<<2) | symb->position;
}

void strToBinWord(char *str, AddressModeType mode, int op_type, int passage, enum ErrorTypes * errCode) {
    int bits = 0;
    static struct {
        unsigned is_srcop_reg:2;
        unsigned is_destop_reg:2;
    } flag = {FALSE, FALSE};
    if (op_type == SRC_OP) flag.is_srcop_reg = FALSE;
    symbolTable *symbolId;
    char label[MAX_LINE] = "";
    char arg1[MAX_LINE], arg2[MAX_LINE];

    switch (mode) {
        case ADDMODE_IMMEDIATE:
            bits = atoi(&str[1]);
            if (bits < 0)
                bits = ~((-1) * bits) + 1;
            code[++ic] |= bits << 2;
            break;
        case ADDMODE_DIRECT:
            if (passage == SECOND_PASS) { /* check if symbol exists at 2nd pass */
                if((symbolId = get_symbolId(str)) != NULL)
                    code[++ic] |= symbToBin(symbolId);
                else
                    *errCode = E_UNDECLARED_SYMBOL;
            }
            else
                code[++ic] |= 0;
            break;
        case ADDMODE_MATRIX:
            strncpy(label, str, strchr(str, '[') - str);
            if (passage == FIRST_PASS)
                ic++;       /* no label adress yet, progressing one step */
            else if (passage == SECOND_PASS) {
                code[++ic] |= symbToBin(get_symbolId(label));
            }
            cpyMatVals(str, arg1, arg2);
            code[++ic] |= (atoi(&arg1[1]) << 6) | atoi(&arg2[1]) << 2;
            break;
        case ADDMODE_REG:
            if (op_type == SRC_OP)
                flag.is_srcop_reg = TRUE, ic++;
            if (op_type == DEST_OP && flag.is_srcop_reg != TRUE)
                ic++;
            bits = atoi(&str[1]);
            code[ic] |= bits << (op_type == SRC_OP ? 6 : 2);
            break;
        case ADDMODE_NO_OPERAND:
            code[ic] |= 0;
        default:
            return;
    }

}

void set_offset(void){
    offset = ic-STARTING_ADD;
}

enum ErrorTypes updateIc(char *cmd, char *src_op, char *dest_op, int passage) {
    enum ErrorTypes errCode = NO_ERR_OCCURRED;
    int word, i;
    char *str;
    AddressModeType srcop_mode, destop_mode;

    destop_mode = getAddMode(dest_op);
    srcop_mode = getAddMode(src_op);

    if ((errCode = isValidAddressMode(cmd, srcop_mode, destop_mode)) != NO_ERR_OCCURRED)
        return errCode;

    /* coding the command to a bit-word and updating the array */
    for (i = 0; i < NUM_OF_CMDS; i++) {
        if (strcmp(cmd, COMMANDS[i].cmd) == 0) {
            code[ic] |= COMMANDS[i].code << 6;  /* 6-9 bits */
            break;
        }
    }
    code[ic] |= (srcop_mode == ADDMODE_NO_OPERAND ? 0 : srcop_mode) << 4;   /* 4-5 bits */
    code[ic] |= (destop_mode == ADDMODE_NO_OPERAND ? 0 : destop_mode) << 2;     /* 2-3 bits */


    strToBinWord(src_op, srcop_mode, SRC_OP, passage, &errCode);      /* inserts source operand as a bit-word */
    strToBinWord(dest_op, destop_mode, DEST_OP, passage, &errCode);       /* inserts dest operand as a bit-word */

    ic++;
    return errCode;
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

/* to use the tester, create 3 files that should be tested against. with a postfix of .test
         * for example filename.as.test or filename.ent.test */
void test(char *filename1){
    int i = 3,j = 0;
    char str[10];
    char filename[20000] ="";
    char s1[10000],s2[10000];
    FILE *testob1,*testob2;

    while(i) {
        strcpy(filename, filename1);
        if(i == 3)
            strcpy(str,".ob");
        if(i == 2)
            strcpy(str,".ent");
        if(i == 1)
            strcpy(str,".ext");
        testob1 = fopen(strcat(filename, str), "r");
        testob2 = fopen(strcat(filename, ".test"), "r");
        j = STARTING_ADD;
        if (testob1 == NULL || testob2 == NULL)
            exit(1);
        printf("\n****************************************************\n\nComparing %s Files:\n\n",&str[1]);
        while (fgets(s1, 100, testob1) && fgets(s2, 100, testob2)) {
            \
        if (strchr(s1, '\n'))
                *strchr(s1, '\n') = '\0';
            if (strchr(s2, '\n'))
                *strchr(s2, '\n') = '\0';

            printf("%d: (%s)\t(%s)\t", j, s1, s2);
            if (strcmp(s1, s2) != 0)
                printf("** No Match **\n");
            else
                printf("**  MATCH  **\n");

            j++;
        }
        i--;
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
    char outFileName[FILENAME_MAX];
    char word[WORD_LEN + 1];
    char address[ADDRESS_LEN + 1];
    strcpy(outFileName,fileName);
    int i;
    if((outf = fopen(strcat(outFileName,OUT_OB),"w")) == NULL)
    {
        LOG_TRACE(LOG_ERROR, "Could not write to %s","%s");
        exit(EXIT_FAILURE);
    }
    for(i = STARTING_ADD;i<ic;i++){
        binToWierdFour(i,address,ADDRESS_LEN);
        fprintf(outf,"%.4s\t",address);
        binToWierdFour(code[i],word,WORD_LEN);
        fprintf(outf,"%.5s\n",word);
    }
    for(i = STARTING_ADD;i<dc;i++){
        binToWierdFour(i+offset,address,ADDRESS_LEN);
        fprintf(outf,"%.4s\t",address);
        binToWierdFour(data[i],word,WORD_LEN);
        fprintf(outf,"%.5s\n",word);
    }

    fclose(outf);
}

void create_ext_file(char *fileName){
    int i = 0;
    FILE *outf;
    char outFileName[MAX_FILE_NAME];
    char address[ADDRESS_LEN + 1];
    strcpy(outFileName,fileName);
    if((outf = fopen(strcat(outFileName,OUT_EXT),"w")) == NULL)
    {
        fprintf(stderr,"Could not write to %s","%s");
        exit(EXIT_FAILURE);
    }
    for(i = 0;i<extref_ind;i++) {
        binToWierdFour(extref[i].address,address,ADDRESS_LEN);
        fprintf(outf, "%s\t%s\n", extref[i].label, address);
    }
    fclose(outf);
}

void print_ent(FILE *outf,symbolTable *symnode){
    static char address[ADDRESS_LEN+1];
    if(symnode == NULL)
        return;
    else if(symnode->format == ENTRY) {
        binToWierdFour(symnode->address + (symnode->iscmd == NOT_CMD2 && symnode->position == RELOCATABLE?offset:0),address,ADDRESS_LEN);
        fprintf(outf, "%s\t%s\n", symnode->label,address);
    }
    print_ent(outf,symnode->right);
    print_ent(outf,symnode->left);

}

void create_ent_file(char *fileName){
    FILE *outf;
    char outFileName[MAX_FILE_NAME];
    strcpy(outFileName,fileName);
    if((outf = fopen(strcat(outFileName,OUT_ENT),"w")) == NULL)
    {
        LOG_TRACE(LOG_ERROR, "Could not write to %s","%s");
        exit(EXIT_FAILURE);
    }

    print_ent(outf,symbolTab_head);

    fclose(outf);
}
