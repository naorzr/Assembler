
#ifndef ENDPROJECT_DATA_STRUCT_H
#define ENDPROJECT_DATA_STRUCT_H
#include "assembler.h"
#include "content_validation.h"

#define NUM_OF_REG 8
#define NUM_OF_CMDS 16
#define MAX_STACK_SIZE 256 /* defines the max size for stacks, which means that the data, code, ext ref stacks are limited to 256  */
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

/**
 * Update a symbol with new data. used for updating labels preceded by entry directive
 * @param label
 * @param position - ABSOLUTE/EXTERNAL/RELOCATABLE
 * @param format - ENTRY/NONE ENTRY
 * @param iscmd
 * @return In case of an error returns the specific type of error
 */
ErrorTypes update_sym_table(char *label, int position, int format);



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
ErrorTypes insert_to_symtab(char *label, int address, int position, int format, int iscmd);


/**
 * frees the whole symbol table, send in to the internal function free_symtree the symbol table head.
 */
void free_symbtable(void);



/***********************************************************/
/*              Data Array Handling Functions             *
 * ********************************************************/

/**
 * Getter for the DC address
 * @return
 */
int get_dc(void);

/**
 * Updates the data array with the binary representation of the given operand
 * @param directive directive name
 * @param op_string a string containing all the directive operands
 * @return In case of an error returns the specific type of error
 */
ErrorTypes update_data(char *directive, char *op2);


/**
 * Clears the data stacks
 */
void clear_data_stacks(void);

/**
 * Sets the offset value for the relocatable labels address
 */
void set_offset(void);

/***********************************************************/
/*              Code Array Handling Functions             *
 * ********************************************************/


/**
 * Checks the validity of the address mode according to the command rules,
 * and updates the code arr with the binary code of the cmd+op if no errors occurred.
 * @param cmd
 * @param src_op - source operand to process
 * @param dest_op - destination operand to process
 * @param passage - FIRST/SECOND passage
 * @return In case of an error returns the specific type of error
 */
ErrorTypes update_code(char *cmd, char *op1, char *op2, int state);

/**
 * Getter for the IC address
 * @return
 */
int get_ic(void);

/**
 * Clears the code array
 */
void clear_code_arr(void);


/***********************************************************/
/*              File Exporting Functions                   *
 * ********************************************************/

/**
 * Creates assembler object file - all data is presented as weird 4
 * @param fileName - file name for exporting
 */
void create_ob_file(char *outf);

/**
 * Creates assembler entry file - all data is presented as weird 4
 * @param fileName - file name for exporting
 */
void create_ent_file(char *outf);

/**
 * Creates assembler external file - all data is presented as weird 4
 * @param fileName - file name for exporting
 */
void create_ext_file(char *outf);


/***********************************************************/
/*              Miscellaneous Functions                    *
 * ********************************************************/

/**
 * Frees the external reference
 */
void free_ext_ref(void);



void test(char *filename);

#endif /*ENDPROJECT_DATA_STRUCT_H*/