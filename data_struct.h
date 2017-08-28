
#ifndef ENDPROJECT_DATA_STRUCT_H
#define ENDPROJECT_DATA_STRUCT_H
#include "assembler.h"
#include "content_validation.h"

/* TODO: IMPORTANT!!! 23/8/17 need to check if it is allowed to define max file size or only max line */
#define LOOP  for(;;)
#define NUM_OF_REG 8
#define NUM_OF_CMDS 16
#define MAX_FILE_SIZE 256
#define WORD_LEN 5
#define ADDRESS_LEN 4
#define STARTING_ADD 100
#define OUT_EXT ".ext"
#define OUT_ENT ".ent"
#define OUT_OB ".ob"
#define IN_EXT ".as"
#define MAX_FILE_NAME 256
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
enum {EXTERNAL_ADDRESS = 0, ENTRY=10, NONE_ENTRY = 11,CMD2=4, NOT_CMD2=5};

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