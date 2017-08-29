
#ifndef ENDPROJECT_CONTENT_VALIDATION_H
#define ENDPROJECT_CONTENT_VALIDATION_H

#include <string.h>
#include "data_struct.h"

enum {OFF};
enum {FALSE,TRUE};
/* Enums for the different addressing modes */
typedef enum AddressModeType{
    ADDMODE_INVALID = -2,
    ADDMODE_NO_OPERAND = -1,
    ADDMODE_IMMEDIATE = 0,
    ADDMODE_DIRECT = 1,
    ADDMODE_MATRIX = 2,
    ADDMODE_REG = 3
} AddressModeType;

#define Is_External(label) strcmp((label),"extern") == 0        /* Checks if a string is an external */
#define Is_Entry(label)     strcmp((label),"entry") == 0        /* Checks if a string is an entry */
#define LABEL_DEC(word)    (word)[(strlen((word)))-1] == ':'    /* Checks if a string is a label */
#define Is_Directive(word)     (word)[0] == '.'                 /* Checks if a string is a directive */
#define MIN_POS_INT 0
#define MAX_POS_INT 512                                         /* 10bit limitation in this assignment is between -511-511*/
#define MAX_NEG_INT -512                                        /* 10bit limitation in this assignment is between -511-511*/
#define MAX_LABEL_SIZE 30



int is_label(char *label);

int is_dsm(char *word);

int is_cmd(char *word);

int is_reg(char *word);

int is_string(char *str);

int is_num(char *str);

int valid_pos_num(char *str);

int valid_num_val(int num);

int valid_mat_init(char *mat);

int is_mat(char *str);

int validate_commas(char *str);

int is_immediate(char *op);

ErrorTypes valid_address_mode(char *cmd,AddressModeType src_op, AddressModeType dest_op);


#endif /*ENDPROJECT_CONTENT_VALIDATION_H*/