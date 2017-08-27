


#include <string.h>
#include <stdio.h>
#include "content_validation.h"
#include "helpers.h"
#include "logger.h"

/**
 * Initialize the command with the appropriate addressing modes rules
 * Every command is initialized in the following order:
 * e.g.
 * {
 *   "mov",              The command name
 *   0,                  Command code
 *   {                   Command first operand rules
 *      0,               No operand - FALSE
 *      1,               immediate - TRUE
 *      1,               direct - TRUE
 *      1,               matrix - TRUE
 *      1                register - TRUE
 *   },
 *   {                   Command 2nd operand rules
 *      0,               No operand - FALSE
 *      0,               immediate - FALSE
 *      1,               direct - TRUE
 *      1,               matrix - TRUE
 *      1                register - TRUE
 *   }
 * }
 */
const struct Command COMMANDS[] = {
        {"mov",  0,  {0, 1, 1, 1, 1}, {0, 0, 1, 1, 1}},
        {"cmp",  1,  {0, 1, 1, 1, 1}, {0, 1, 1, 1, 1}},
        {"add",  2,  {0, 1, 1, 1, 1}, {0, 0, 1, 1, 1}},
        {"sub",  3,  {0, 1, 1, 1, 1}, {0, 0, 1, 1, 1}},
        {"not",  4,  {1, 0, 0, 0, 0}, {0, 0, 1, 1, 1}},
        {"clr",  5,  {1, 0, 0, 0, 0}, {0, 0, 1, 1, 1}},
        {"lea",  6,  {0, 0, 1, 1, 0}, {0, 0, 1, 1, 1}},
        {"inc",  7,  {1, 0, 0, 0, 0}, {0, 0, 1, 1, 1}},
        {"dec",  8,  {1, 0, 0, 0, 0}, {0, 0, 1, 1, 1}},
        {"jmp",  9,  {1, 0, 0, 0, 0}, {0, 0, 1, 1, 1}},
        {"bne",  10, {1, 0, 0, 0, 0}, {0, 0, 1, 1, 1}},
        {"red",  11, {1, 0, 0, 0, 0}, {0, 0, 1, 1, 1}},
        {"prn",  12, {1, 0, 0, 0, 0}, {0, 1, 1, 1, 1}},
        {"jsr",  13, {1, 0, 0, 0, 0}, {0, 0, 1, 1, 1}},
        {"rts",  14, {1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}},
        {"stop", 15, {1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}}
};

/* strToBinWord: internal function, converts a string to a binary word */
void strToBinWord(char *str, AddressModeType mode, int op_type, int passage, ErrorTypes * errCode);
const char *const REGISTERS[NUM_OF_REG] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};


static symbolTable *symbolTab_head = NULL,*symbolTab_tail = NULL;


static unsigned dc = STARTING_ADD;
static unsigned ic = STARTING_ADD;
static unsigned extref_ind = 0;
static unsigned offset = 0;

static unsigned code[MAX_FILE_SIZE] = {0};
static unsigned data[MAX_FILE_SIZE] = {0};
static struct{
    const char * label;
    unsigned int address;
} extref[MAX_FILE_SIZE] = {0};



/***********************************************************/
/*              Symbol Table Handling Functions             *
 * ********************************************************/


/**
 * Updates the symbols table with a new symbol.
 * Symbol Table is represented as a binary search tree, whereas each node contains all the symbol data
 * @param label
 * @param address
 * @param position
 * @param format
 * @param iscmd
 * @return
 */
ErrorTypes updateSymbolTable(char *label, int address, int position, int format, int iscmd) {
    symbolTable *node;
    node = NEW_SYMTABLE_NODE(label, address, position, format, iscmd)

    if (symbolTab_head == NULL) {
        symbolTab_tail = symbolTab_head = node;
        return NO_ERR_OCCURRED;
    }

    symbolTab_tail = symbolTab_head;

    LOOP {
        if (strcmp(symbolTab_tail->label, node->label) == 0) {      /* case a symbol was found */
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





/***********************************************************/
/*              Data Array Handling Functions             *
 * ********************************************************/


ErrorTypes updateData(char *directive,char *op_string){

    char arg1[MAX_LINE],arg2[MAX_LINE];
    char *param;
    int bitword, mat_word_size = 0;
    if(strcmp(directive,"data") == 0){
        param = safe_strtok(op_string,",NULL");

        do {
            if (param == NULL || !is_num(param)) {
                return ERR_INV_DATA_PARAM;
            }
            bitword = atoi(param);
            if (!valid_num_val(bitword)) {
                return ERR_INV_DATA_SIZE;
            }
            if (bitword < 0)
                bitword = ~(bitword * -1) + 1;
            data[dc++] = (unsigned) bitword;
        } while ((param = safe_strtok(NULL,",NULL")) != NULL);

    } else if(strcmp(directive,"string") == 0){
        param = safe_strtok(op_string,"");
        if(param == NULL || !is_string(param)){
            return ERR_INV_STRING_PARAM;
        }
        param[strlen(param)-1] = '\0';
        param = &param[1];
        while(*param)
            data[dc++] = (unsigned) *(param++);

        data[dc++] = '\0';

    } else if(strcmp(directive,"mat") == 0){
        param = safe_strtok(op_string," ");
        if(!valid_mat_init(param)){
            return ERR_INV_MAT_INIT;
        }
        cpy_mat_vals(param,arg1,arg2);
        mat_word_size = atoi(arg1)*atoi(arg2);

        while ((param = safe_strtok(NULL,",NULL")) != NULL){
            if(mat_word_size <= 0)
                return ERR_EXSS_MAT_VAL;
            mat_word_size--;
            if (!is_num(param)) {
                return ERR_INV_MAT_PARAM;
            }
            bitword = atoi(param);
            if (!valid_num_val(bitword)) {
                return ERR_INV_MAT_PARAM_SIZE;
            }
            if (bitword < 0)
                bitword = ~(bitword * -1) + 1;
            data[dc++] = (unsigned) bitword;
        }
        while(mat_word_size--)
            dc++;
    }
    return NO_ERR_OCCURRED;
}


int getDc(void){
    return dc;
}


void set_offset(void){
    offset = ic-STARTING_ADD;
}


void clear_data_stacks(void){
    while(dc > STARTING_ADD)
        data[dc--] = 0;
    while(ic > STARTING_ADD)
        data[ic--] = 0;
    data[dc] = 0;
    data[ic] = 0;
}

/***********************************************************/
/*              Code Array Handling Functions             *
 * ********************************************************/


ErrorTypes updateIc(char *cmd, char *src_op, char *dest_op, int passage) {
    ErrorTypes errCode;
    AddressModeType srcop_mode, destop_mode;
    int i;

    destop_mode = getAddMode(dest_op);
    srcop_mode = getAddMode(src_op);

    if ((errCode = valid_address_mode(cmd, srcop_mode, destop_mode)) != NO_ERR_OCCURRED)
        return errCode;

    /* coding the command to a bit-word and updating the array */
    for (i = 0; i < NUM_OF_CMDS; i++) {
        if (strcmp(cmd, COMMANDS[i].cmd) == 0) {
            code[ic] |= COMMANDS[i].code << 6;  /* 6-9 bits */
            break;
        }
    }
    code[ic] |= (srcop_mode == ADDMODE_NO_OPERAND ? 0 : (int)srcop_mode) << 4;   /* 4-5 bits */
    code[ic] |= (destop_mode == ADDMODE_NO_OPERAND ? 0 : (int)destop_mode) << 2;     /* 2-3 bits */


    strToBinWord(src_op, srcop_mode, SRC_OP, passage, &errCode);      /* inserts source operand as a bit-word */
    strToBinWord(dest_op, destop_mode, DEST_OP, passage, &errCode);       /* inserts dest operand as a bit-word */

    ic++;
    return errCode;
}

int getIc(void){
    return ic;
}


/**
 * Clears the code array
 */
void clear_code_arr() {
    memset(code, 0, sizeof(code));
}



/***********************************************************/
/*              File Exporting Functions                   *
 * ********************************************************/


void create_ob_file(char *fileName){
    FILE *outf;
    unsigned i;
    char outFileName[FILENAME_MAX];
    char word[WORD_LEN + 1];
    char address[ADDRESS_LEN + 1];
    strcpy(outFileName,fileName);

    if((outf = fopen(strcat(outFileName,OUT_OB),"w")) == NULL)
    {
        LOG_TRACE(LOG_ERROR, "Could not write to %s","%s");
        exit(EXIT_FAILURE);
    }
    for(i = STARTING_ADD;i<ic;i++){
        binToWeirdFour(i,address,ADDRESS_LEN);
        fprintf(outf,"%.4s\t",address);
        binToWeirdFour(code[i],word,WORD_LEN);
        fprintf(outf,"%.5s\n",word);
    }
    for(i = STARTING_ADD;i<dc;i++){
        binToWeirdFour(i+offset,address,ADDRESS_LEN);
        fprintf(outf,"%.4s\t",address);
        binToWeirdFour(data[i],word,WORD_LEN);
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
        binToWeirdFour(extref[i].address,address,ADDRESS_LEN);
        fprintf(outf, "%s\t%s\n", extref[i].label, address);
    }
    fclose(outf);
}

void print_ent(FILE *outf,symbolTable *symnode){
    static char address[ADDRESS_LEN+1];
    if(symnode == NULL)
        return;
    else if(symnode->format == ENTRY) {
        binToWeirdFour(symnode->address + (symnode->iscmd == NOT_CMD2 && symnode->position == RELOCATABLE?offset:0),address,ADDRESS_LEN);
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


/***********************************************************/
/*              Miscellaneous Functions                    *
 * ********************************************************/



void strToBinWord(char *str, AddressModeType mode, int op_type, int passage, ErrorTypes * errCode) {
    char label[MAX_LINE] = "";
    char arg1[MAX_LINE], arg2[MAX_LINE];
    symbolTable *symbolId;
    int bits = 0;
    static struct {
        unsigned is_srcop_reg:2;
        unsigned is_destop_reg:2;
    } flag = {FALSE, FALSE};

    if (op_type == SRC_OP) flag.is_srcop_reg = FALSE;

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
            cpy_mat_vals(str, arg1, arg2);
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

/**
 * Frees the external reference
 */
void freeExtRef() {
    extref_ind = 0;
    memset(&extref, 0, sizeof(extref));
}


/********************************************************************************************/

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
            return;
        printf("\n****************************************************\n\nComparing %s %s Files:\n\n",filename1,&str[1]);
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
        fclose(testob1);
        fclose(testob2);
    }
    printf("\n");

}





