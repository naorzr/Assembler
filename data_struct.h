
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
#define NEW_SYMTABLE_NODE(lab,add,position,format,iscmd)  (symbolTable *) safe_malloc(sizeof(symbolTable));\
                                                strcpy(node->label,(lab));\
                                                node->address = (add);\
                                                node->position = (position);\
                                                node->format = (format);\
                                                node->iscmd = (iscmd);\
                                                node-> right = node->left = NULL;\


enum CmdOpType{SRC_OP,DEST_OP};

enum Positions{ABSOLUTE,EXTERNAL,RELOCATABLE};
enum {EXTERNAL_ADDRESS = 0, ENTRY=10, NONE_ENTRY,CMD2=4, NOT_CMD2=5};

/* symbole table node */
typedef struct symbolTable{
    char label[MAX_LINE];
    int address;
    int position;
    int format;
    int iscmd;
    struct symbolTable *left;
    struct symbolTable *right;
}symbolTable;


typedef struct addressingMode {
    unsigned noOperand: 2,
            immediate: 2,
            direct: 2,
            matrix: 2,
            reg: 2;
} addressingMode;

struct Command{
    char *cmd;
    int code;
    addressingMode addressingMode_op1;
    addressingMode addressingMode_op2;
};

/***********************************************************/
/*              Symbol Table Handling Functions             *
 * ********************************************************/

ErrorTypes updateSymbolTable(char *label,int address,int storageType,int isentry,int iscmd);

void free_symbtable(void);



/***********************************************************/
/*              Data Array Handling Functions             *
 * ********************************************************/

ErrorTypes updateData(char *directive,char *op2);

int getDc(void);

void clear_data_stacks(void);

void set_offset(void);

/***********************************************************/
/*              Code Array Handling Functions             *
 * ********************************************************/

ErrorTypes updateIc(char *cmd,char *op1,char *op2,int state);

int getIc(void);

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
void freeExtRef(void);



void test(char *filename);

#endif /*ENDPROJECT_DATA_STRUCT_H*/