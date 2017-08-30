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

/* cmdop_to_bin: internal function, converts a string to a binary word */
void cmdop_to_bin(char *str, AddressModeType mode, int op_type, int passage, ErrorTypes *errCode);
/* List of available registers r0-r7 */
const char *const REGISTERS[NUM_OF_REG] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};


static SymbolTable *symbolTab_head = NULL;


static unsigned dc = 0;      /* data array address */
static unsigned ic = 0;      /* code array address */
static unsigned code[MAX_STACK_SIZE] = {0};  /* code array, stores all the command sentences data */
static unsigned data[MAX_STACK_SIZE] = {0};  /* data array, stores all the directive sentences data */
static unsigned offset;     /* offset value used for adjusting data array address */
static struct{
    const char * label;
    unsigned int address;
} extref[MAX_STACK_SIZE] = {{0}};      /* stores the label and its appearences */
static unsigned extref_ind = 0;



/***********************************************************/
/*              Symbol Table Handling Functions             *
 * ********************************************************/



/**
 * Inserts a new node into the symbol table
 * symbol table is represented as a binary search tree.
 * @param label
 * @param address - ic or dc address
 * @param position - ABSOLUTE/EXTERNAL/RELOCATABLE
 * @param format - ENTRY/NONE ENTRY
 * @param iscmd
 * @return In case of an error returns the specific type of error
 */
ErrorTypes insert_to_symtab(char *label, int address, int position, int format, int iscmd) {
    SymbolTable *node,*ptr;
    node = NEW_SYMTABLE_NODE(label, address, position, format, iscmd)

    if (symbolTab_head == NULL) {
        symbolTab_head = node;
        return NO_ERR_OCCURRED;
    }

    ptr = symbolTab_head;

    /* inserting the node into the bin-tree */
    while(ptr != NULL) {
        /* case a symbol with identical name was found will be considered an error unless
         * it is an entry/extern declaration*/
        if (strcmp(ptr->label, node->label) == 0) {
            return E_LABEL_REDECLARED;
        }
        else if (strcmp(ptr->label, node->label) < 0) {
            if (ptr->left == NULL) {
                ptr->left = node;
                return NO_ERR_OCCURRED;
            } else
                ptr = ptr->left;
        } else {
            if (ptr->right == NULL) {
                ptr->right = node;
                return NO_ERR_OCCURRED;
            } else
                ptr = ptr->right;
        }
    }
    return E_UNDECLARED_SYMBOL;

}


/**
 * Update a symbol with new data. used for updating labels preceded by entry directive
 * @param label
 * @param position - ABSOLUTE/EXTERNAL/RELOCATABLE
 * @param format - ENTRY/NONE ENTRY
 * @param iscmd
 * @return In case of an error returns the specific type of error
 */
ErrorTypes update_sym_table(char *label, int position, int format) {
    SymbolTable *ptr;

    ptr = symbolTab_head;

    while (ptr != NULL) {
        if (strcmp(ptr->label,label) == 0) {
            /* updating label position and format */
            ptr->format = format;
            ptr->position |= position;
            return NO_ERR_OCCURRED;
        }
        else if (strcmp(ptr->label,label) < 0) {
            ptr = ptr->left;
        } else {
            ptr = ptr->right;
        }
    }
    return E_UNDECLARED_SYMBOL;

}


/** get_symbolId: get the user the symbol node matching the label passed
 * @param label label's name to search for
 * @return symbol table node in case it has found a match, NULL otherwise
 * */
SymbolTable *get_symbolId(char *label){
    SymbolTable *node = symbolTab_head;
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

/**
 * convert the symbol node into the appropriate binary code
 * @param symb a symbol node to convert
 * @return a binary representation of the symbol node address and position in binary
 */
int sym_to_bin(SymbolTable *symb) {
    int address = 0;

    if (symb->position == EXTERNAL) {       /* if it's an external symbol updates the place it was used in the file in the extref array */
        extref[extref_ind].address = ic + STARTING_ADD + 1;        /* place it was used in file */
        extref[extref_ind].label = symb->label;
        extref_ind++;
    }

    /* if its a directive sentence with relocatable position, offset the address */
    address = symb->address + (symb->iscmd == NOT_CMD2 && symb->position == RELOCATABLE ? offset : 0);
    return (address << 2) | symb->position;
}


/**
 * internal function, frees the symboltable from a given point
 * @param node head of a binary tree to free
 */
static void free_symtree(SymbolTable* node) {
    if (node == NULL)
        return;
    if (node->right == NULL && node->left == NULL)
        free(node);
    else {
        free_symtree(node->right);
        free_symtree(node->left);
    }
}


/**
 * frees the whole symbol table, send in to the internal function free_symtree the symbol table head.
 */
void free_symbtable(void){
    free_symtree(symbolTab_head);
    symbolTab_head = NULL;
}





/***********************************************************/
/*              Data Array Handling Functions             *
 * ********************************************************/


/**
 * setting data array value in a specific index
 * @param index - index between 0 to MAX_STACK_SIZE
 * @param value - to store inside the code array
 * @param err_code
 */
void set_data_val(int index,int value,ErrorTypes *err_code){
    if(index >= MAX_STACK_SIZE)
        *err_code = DATA_STACK_OVERFLOW;
    data[index] |= value;
}


/**
 * Getter for the DC address
 * @return
 */
int get_dc(void){
    return dc+STARTING_ADD;
}

/**
 * Updates the data array with the binary representation of the given operand
 * @param directive directive name
 * @param op_string a string containing all the directive operands
 * @return In case of an error returns the specific type of error
 */
ErrorTypes update_data(char *directive, char *op_string) {
    ErrorTypes errCode = NO_ERR_OCCURRED;
    char arg1[MAX_LINE], arg2[MAX_LINE];    /* will be used for matrix values in case needed */
    char *param;
    int bitword, mat_word_size = 0;

    /* case its a data directive */
    if (strcmp(directive, "data") == 0) {
        param = safe_strtok(op_string, ",NULL");
        /* loop 'till run out of parameters and updating each of them in the data array as a binary code */
        do {
            if (param == NULL || !is_num(param)) {
                return E_INV_DATA_PARAM;
            }

            bitword = atoi(param);
            if (!valid_num_val(bitword)) {
                return E_INV_DATA_SIZE;
            }

            if (bitword < 0)
                bitword = ~(bitword * -1) + 1;      /* 2's complement representation */
            set_data_val(dc++,(unsigned) bitword,&errCode);

        } while ((param = safe_strtok(NULL, ",NULL")) != NULL);

        /* case its a string directive */
    } else if (strcmp(directive, "string") == 0) {
        param = safe_strtok(op_string, "");
        if (param == NULL || !is_string(param)) {
            return E_INV_STRING_PARAM;
        }
        /* omitting the quotation marks of the string */
        param[strlen(param) - 1] = '\0';
        param = &param[1];

        while (*param)
            set_data_val(dc++, (unsigned) *(param++) ,&errCode);     /* update data array with the ascii coding binary*/

        set_data_val(dc++, 0, &errCode);     /* terminating null ascii representation */

        /* case its a matrix directive */
    } else if (strcmp(directive, "mat") == 0) {
        param = safe_strtok(op_string, " ");

        if (!valid_mat_init(param)) {
            return E_INV_MAT_INIT;
        }

        cpy_mat_vals(param, arg1, arg2);
        mat_word_size = atoi(arg1) * atoi(arg2);

        while ((param = safe_strtok(NULL, ",NULL")) != NULL) {
            if (mat_word_size <= 0)
                return E_EXSS_MAT_VAL;      /* more parameters than matrix size */
            mat_word_size--;

            if (!is_num(param)) {
                return E_INV_MAT_PARAM;
            }
            bitword = atoi(param);
            if (!valid_num_val(bitword)) {
                return E_INV_MAT_PARAM_SIZE;
            }

            if (bitword < 0)
                bitword = ~(bitword * -1) + 1;  /* 2's complement representation */

            set_data_val(dc++, (unsigned) bitword, &errCode);
        }
        while (mat_word_size--)     /* partial initialization(e.g .mat [2][3] 1,2,1) will result in zeros fillings */
            dc++;
    }

    return errCode;
}


/**
 * Sets the offset value for the relocatable labels address
 */
void set_offset(void){
    offset = ic;
}

/**
 * Clears the data stacks
 */
void clear_data_stacks(void){
    while(dc > 0)
        data[dc--] = 0;
    while(ic > 0)
        data[ic--] = 0;
    data[dc] = 0;
    data[ic] = 0;
}

/***********************************************************/
/*              Code Array Handling Functions             *
 * ********************************************************/


/**
 * setting code array value in a specific index
 * @param index - index between 0 to MAX_STACK_SIZE
 * @param value - to store inside the code array
 * @param err_code
 */
void set_code_val(int index,int value,ErrorTypes *err_code){
    if(index >= MAX_STACK_SIZE)
        *err_code = CODE_STACK_OVERFLOW;
    code[index] |= value;
}

/**
 * Getter for the IC address
 * @return
 */
int get_ic(void){
    return ic+STARTING_ADD;
}

/**
 * Checks the validity of the address mode according to the command rules,
 * and updates the code arr with the binary code of the cmd+op if no errors occurred.
 * @param cmd
 * @param src_op - source operand to process
 * @param dest_op - destination operand to process
 * @param passage - FIRST/SECOND passage
 * @return In case of an error returns the specific type of error
 */
ErrorTypes update_code(char *cmd, char *src_op, char *dest_op, int passage) {
    ErrorTypes errCode;
    AddressModeType srcop_mode, destop_mode;
    int i;

    destop_mode = get_add_mode(dest_op);
    srcop_mode = get_add_mode(src_op);

    if ((errCode = valid_address_mode(cmd, srcop_mode, destop_mode)) != NO_ERR_OCCURRED)
        return errCode;


    /* coding the command to a bit-word and updating the array */
    for (i = 0; i < NUM_OF_CMDS; i++) {
        if (strcmp(cmd, COMMANDS[i].cmd) == 0) {
            set_code_val(ic,COMMANDS[i].code << 6,&errCode);  /* 6-9 bits */
            break;
        }
    }
    set_code_val(ic,(srcop_mode == ADDMODE_NO_OPERAND ? 0 : (int)srcop_mode) << 4,&errCode);   /* 4-5 bits */
    set_code_val(ic,(destop_mode == ADDMODE_NO_OPERAND ? 0 : (int)destop_mode) << 2,&errCode);     /* 2-3 bits */

    /* passing the command operand to the internal function cmdop_to_bin to process the operand
     * converting it to binary and storing it into the code array */
    cmdop_to_bin(src_op, srcop_mode, SRC_OP, passage, &errCode);
    cmdop_to_bin(dest_op, destop_mode, DEST_OP, passage, &errCode);

    ic++;
    return errCode;
}



/**
 * Clears the code array
 */
void clear_code_arr(void) {
    memset(code, 0, sizeof(code));
}



/***********************************************************/
/*              File Exporting Functions                   *
 * ********************************************************/

/**
 * Creates assembler object file - all data is presented as weird 4
 * @param fileName - file name for exporting
 */
void create_ob_file(char *fileName) {
    char outFileName[FILENAME_MAX];
    char word[WORD_LEN + 1];
    char address[ADDRESS_LEN + 1];
    unsigned i;
    FILE *outf;
    strcpy(outFileName, fileName);

    if ((outf = fopen(strcat(outFileName, OUT_OB), "w")) == NULL)
        log_trace(LOG_ERROR, "Could not write to %s", outFileName);
    else {
        /* traverse code array, convert and stores the data from binary to weird 4 in an output file*/
        for (i = 0; i < ic; i++) {
            bin_to_weird4(i + STARTING_ADD, address, ADDRESS_LEN);
            fprintf(outf, "%.4s\t", address);
            bin_to_weird4(code[i], word, WORD_LEN);
            fprintf(outf, "%.5s\n", word);
        }
        /* traverse data array, convert and stores the data from binary to weird 4 in an output file*/
        for (i = 0; i < dc; i++) {
            bin_to_weird4(i + STARTING_ADD + offset, address, ADDRESS_LEN);
            fprintf(outf, "%.4s\t", address);
            bin_to_weird4(data[i], word, WORD_LEN);
            fprintf(outf, "%.5s\n", word);
        }
        fclose(outf);
    }
}

/**
 * Creates assembler external file - all data is presented as weird 4
 * @param fileName - file name for exporting
 */
void create_ext_file(char *fileName) {
    char outFileName[MAX_FILE_NAME];
    char address[ADDRESS_LEN + 1];
    int i = 0;
    FILE *outf;
    strcpy(outFileName, fileName);

    if ((outf = fopen(strcat(outFileName, OUT_EXT), "w")) == NULL)
        fprintf(stderr, "Could not write to %s", "%s");
    else {
        /* traverse extref and print all the data into output file as weird 4 */
        for (i = 0; i < extref_ind; i++) {
            bin_to_weird4(extref[i].address, address, ADDRESS_LEN);
            fprintf(outf, "%s\t%s\n", extref[i].label, address);
        }
        fclose(outf);
    }
}

/**
 * Prints the entries symbols from the symbol table into the entry file
 * @param outf output file stream
 * @param symnode symbol table
 */
void print_ent(FILE *outf, SymbolTable *symnode) {
    static char address[ADDRESS_LEN + 1];
    if (symnode == NULL)
        return;
    else if (symnode->format == ENTRY) {
        bin_to_weird4(symnode->address  + (symnode->iscmd == NOT_CMD2 && symnode->position == RELOCATABLE ? offset : 0),
                      address,
                      ADDRESS_LEN);
        fprintf(outf, "%s\t%s\n", symnode->label, address);
    }
    print_ent(outf, symnode->right);
    print_ent(outf, symnode->left);
}

/**
 * Creates assembler entry file - all data is presented as weird 4
 * @param fileName - file name for exporting
 */
void create_ent_file(char *fileName) {
    FILE *outf;
    char outFileName[MAX_FILE_NAME];
    strcpy(outFileName, fileName);
    if ((outf = fopen(strcat(outFileName, OUT_ENT), "w")) == NULL)
        log_trace(LOG_ERROR, "Could not write to %s", "%s");
    else {
        print_ent(outf, symbolTab_head);
        fclose(outf);
    }
}


/***********************************************************/
/*              Miscellaneous Functions                    *
 * ********************************************************/


/**
 * Command operator to binary. takes in a
 * @param operand operand to convert to binary
 * @param mode  address mode of the current operand(immediate,direct etc)
 * @param op_type Operand type - source/destination
 * @param passage first/second
 * @param errCode to store errors if found
 */
void cmdop_to_bin(char *operand, AddressModeType mode, int op_type, int passage, ErrorTypes *errCode) {
    char label[MAX_LINE] = "";
    char arg1[MAX_LINE], arg2[MAX_LINE];
    SymbolTable *symbolId;
    int bits = 0;
    /* in case of a register addressing mode both source op and dest op will be saved
     * on the same word(but different bits). to alert this and prevent saving the second register
     * on the next word a flag is switched off and on accordinally */
    static struct {
        unsigned is_srcop_reg:2;
        unsigned is_destop_reg:2;
    } flag = {FALSE, FALSE};

    if (op_type == SRC_OP) flag.is_srcop_reg = FALSE;

    switch (mode) {
        case ADDMODE_IMMEDIATE:
            bits = atoi(&operand[1]);
            if (bits < 0)
                bits = ~((-1) * bits) + 1;  /* 2's complement representation */
            set_code_val(++ic,bits << 2,errCode);
            break;
        case ADDMODE_DIRECT:
            if(passage == FIRST_PASS)
                set_code_val(++ic,0,errCode);
            else {
                if((symbolId = get_symbolId(operand)) != NULL)  /* check if symbol exists at the 2nd pass */
                    set_code_val(++ic,sym_to_bin(symbolId),errCode);
                else
                    *errCode = E_UNDECLARED_SYMBOL;
            }
            break;
        case ADDMODE_MATRIX:
            if (passage == FIRST_PASS)
                set_code_val(++ic,0,errCode);       /* no label adress yet, progressing one step */
            else if (passage == SECOND_PASS) {      /* get the symbol */
                strncpy(label, operand, strchr(operand, '[') - operand);    /* copies the label part */
                if((symbolId = get_symbolId(label)) == NULL) {  /* fetch symbol */
                    *errCode = E_UNDECLARED_SYMBOL;
                    break;
                }
                set_code_val(++ic,sym_to_bin(get_symbolId(label)),errCode);
            }

            cpy_mat_vals(operand, arg1, arg2);
            set_code_val(++ic,(atoi(&arg1[1]) << 6) | atoi(&arg2[1]) << 2,errCode);         /* update code array with both init registers binary representation */
            break;
        case ADDMODE_REG:
            if (op_type == SRC_OP)
                flag.is_srcop_reg = TRUE, ic++;
            if (op_type == DEST_OP && flag.is_srcop_reg != TRUE)        /* if the register is independent , store it in a different word */
                ic++;
            bits = atoi(&operand[1]);
            set_code_val(ic,bits << (op_type == SRC_OP ? 6 : 2),errCode);
            break;
        case ADDMODE_NO_OPERAND:
            set_code_val(ic,0,errCode);
        default:
            return;
    }
}

/**
 * Frees the external reference
 */
void free_ext_ref(void) {
    extref_ind = 0;
    memset(&extref, 0, sizeof(extref));
}

