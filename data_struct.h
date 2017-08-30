
#ifndef ENDPROJECT_DATA_STRUCT_H
#define ENDPROJECT_DATA_STRUCT_H
#include "assembler.h"
#include "content_validation.h"

#define NUM_OF_REG 8        /* Number of registers */
#define NUM_OF_CMDS 16      /* Number of commands */
#define MAX_FILE_SIZE 256
#define WORD_LEN 5          /* weird base 4 word length in the exported file */
#define ADDRESS_LEN 4       /* weird base 4 address length in the exported file */
#define STARTING_ADD 100    /* the starting address of code array(not to be confused with the array index */
#define OUT_EXT ".ext"      /* output postfix for external file */
#define OUT_ENT ".ent"      /* output postfix for entry file */
#define OUT_OB ".ob"        /* output postfix for object file */
#define IN_EXT ".as"        /* input postfix for assembly file */
#define MAX_FILE_NAME 256   /* maximal file name length */

#define NEW_SYMTABLE_NODE(lab,add,position,format,iscmd)  (SymbolTable *) safe_malloc(sizeof(SymbolTable));\
                                                strcpy(node->label,(lab));\
                                                node->address = (add);\
                                                node->position = (position);\
                                                node->format = (format);\
                                                node->iscmd = (iscmd);\
                                                node-> right = node->left = NULL;\


enum CmdOpType{SRC_OP, DEST_OP};

/* Encoding types - Absolute(kept for readability, even though not used)/external/relocatable */
enum Positions{ABSOLUTE, EXTERNAL, RELOCATABLE};
enum {EXTERNAL_ADDRESS, ENTRY, NONE_ENTRY,CMD2, NOT_CMD2};

/* symbol table node */
typedef struct SymbolTable{
    char label[MAX_LINE];
    int address;
    int position;
    int format;
    int iscmd;
    struct SymbolTable *left;
    struct SymbolTable *right;
}SymbolTable;

/* Struct which defines command operation address mode rules */
typedef struct AddressingMode {
    unsigned noOperand: 2,
            immediate: 2,
            direct: 2,
            matrix: 2,
            reg: 2;
} AddressingMode;

/**
 * Struct which defines command data
 * cmd - command name
 * code - commnd code
 * addressingMode_op1 - address mode rules for first operand
 * addressingMode_op2 - address mode rules for 2nd operand
 */
struct Command{
    char *cmd;
    int code;
    AddressingMode addressingMode_op1;
    AddressingMode addressingMode_op2;
};

/***********************************************************/
/*              Symbol Table Handling Functions             *
 * ********************************************************/

ErrorTypes update_sym_table(char *label, int position, int format);

ErrorTypes insert_to_symtab(char *label, int address, int position, int format, int iscmd);

void free_symbtable(void);



/***********************************************************/
/*              Data Array Handling Functions             *
 * ********************************************************/

ErrorTypes update_data(char *directive, char *op2);

int get_dc(void);

void clear_data_stacks(void);

void set_offset(void);

/***********************************************************/
/*              Code Array Handling Functions             *
 * ********************************************************/

ErrorTypes update_code(char *cmd, char *op1, char *op2, int state);

int get_ic(void);

void clear_code_arr(void);


/***********************************************************/
/*              File Exporting Functions                   *
 * ********************************************************/

void create_ob_file(char *outf);

void create_ent_file(char *outf);

void create_ext_file(char *outf);


/***********************************************************/
/*              Miscellaneous Functions                    *
 * ********************************************************/
void free_ext_ref(void);



void test(char *filename);

#endif /*ENDPROJECT_DATA_STRUCT_H*/