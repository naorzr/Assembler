
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
#define MAX_POS_INT 511                                         /* 10bit limitation in this assignment is between -511-511*/
#define MAX_NEG_INT -512                                        /* 10bit limitation in this assignment is between -511-511*/
#define MAX_LABEL_SIZE 30


/**
 * Check if string is label(starts with a letter,
 * contains only alpha numeric characters and is no longer than 30 characters)
 * @param label
 * @return true if string is label
 */
int is_label(char *label);


/**
 * Checks if a word refers to data/string/mat
 * @param word
 * @return true if string is data/string/mat
 */
int is_dsm(char *word);


/**
 * Checks if a string refers to a command name
 * @param word
 * @return true if string is a command name
 */
int is_cmd(char *word);


/**
 * Checks if a given string is a register
 * @param str string to examine
 * @return true if the string was a register
 */
int is_reg(char *word);


/**
 * Checks if a string refers to a string definition in assembly
 * @param str
 * @return true if a given string refers to a string def
 */
int is_string(char *str);

/**
 * Checks if a given string is a number
 * @param str
 * @return true if the given string is a number
 */
int is_num(char *str);

/**
 * Checks if a num is positive and valid (0-511)
 * @param str number as string
 * @return true if number is valid
 */
int valid_pos_num(char *str);

/**
 * Checks if a number has a valid value
 * value is determined by the size of bits that could be stored in a single word
 * in our case its between -512 and 511
 * @param num
 * @return true if it's in the range , false otherwise
 */
int valid_num_val(int num);

/**
 * Checks if the matrix initializers are valid(i.e has valid parentheses and valid arguments)
 * @param mat
 * @return true if the matrix initializers are valid
 */
int valid_mat_init(char *mat);

/**
 * Checks matrix arguments validity
 * @param str matrix string
 * @return true if matrix args are valid
 */
int is_mat(char *str);

/**
 * Checks line's commas validity
 * e.g. cmp r1,,r2 or .mat [2][2] 1,2,3,4, are illigal
 * @param str line as string
 * @return True if the commas in the line are valid
 */
int validate_commas(char *str);

/**
 * Checks if operand is immediate (number which starts with #) address mode
 * e.g. #1, #-1
 * @param op Operand string
 * @return true if the op is immediate address mode
 */
int is_immediate(char *op);

/**
 * Validate if the addressing modes are correct
 * @param cmd command(e.g sub prn etc)
 * @param src_op source operand addressing mode
 * @param dest_op destination operand addressing mode
 * @return In case of an error returns the specific type of error
 */
ErrorTypes valid_address_mode(char *cmd,AddressModeType src_op, AddressModeType dest_op);


#endif /*ENDPROJECT_CONTENT_VALIDATION_H*/