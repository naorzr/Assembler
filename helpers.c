#include <string.h>
#include "helpers.h"
#include "logger.h"

/**
 * Reverses a string
 * @param s the string to be reversed
 * @return the reversed string
 */
char* reverse(char* s)
{
    char* beg = s-1, *end = s, tmp;
    while (*++end);
    while (end-- > ++beg)
    {
        tmp  = *beg;
        *beg = *end;
        *end =  tmp;
    }
    return s;
}

/**
 * Converts binary to weird-4 base
 * 00 - a
 * 01 - b
 * 10 - c
 * 11 - d
 * @param bin
 * @param fourBaseWord
 * @param nmems
 */
void bin_to_weird4(unsigned int bin, char *fourBaseWord, unsigned nmems){
    unsigned mask;
    int i;
    char *head = fourBaseWord;
    for(i = 0, mask = 3;i < nmems;i++,bin>>=2,fourBaseWord++)
        switch(mask&bin){
            case 0:
                *fourBaseWord = 'a';
                break;
            case 1:
                *fourBaseWord = 'b';
                break;
            case 2:
                *fourBaseWord = 'c';
                break;
            case 3:
                *fourBaseWord = 'd';
                break;
            default:
                *fourBaseWord = '\0';
                return;
        }
    *(fourBaseWord) = '\0';
    reverse(head);
}

/**
 * Wrapper for the malloc method. validates memory allocation and prints error if needed
 * @param size Size of type
  */
void *safe_malloc(size_t size){
    void *ptr = malloc(size);
    if(ptr == NULL) {
        log_trace(LOG_ERROR, "Could not allocate memory");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

/**
 * Wrapper for the strok method, keeps the string unmutated
 * @param str String to be breaked into tokens
 * @param delim Delimiter to the next word
 * @return a pointer to the last token found in the string. A null pointer is returned if there are no tokens left to retrieve.
 */
char *safe_strtok(char *str,const char *delim) {
    static char str_cpy[MAX_LINE],*cur;

    if (str != NULL) {
        strncpy(str_cpy, str, MAX_LINE);
        cur = strtok(str_cpy, delim);
    }
    else
        cur = strtok(NULL,delim);

    return cur;
}


/**
 * Gets the operand address mode
 * @param op
 * @return the op address mode(e.g IMMEDIATE,DIRECT,REGISTER etc)
 */
AddressModeType get_add_mode(char *op) {
    if (strcmp(op, "") == 0)
        return ADDMODE_NO_OPERAND;
    else if (op[0] == '#')
        return is_immediate(op + 1) == TRUE ? ADDMODE_IMMEDIATE : ADDMODE_INVALID;
    else if (is_label(op))
        return ADDMODE_DIRECT;
    else if (is_reg(op))
        return ADDMODE_REG;
    else {
        char label[MAX_LINE] = "";
        char *str;
        str = strchr(op, '[');
        if (str == NULL)
            return ADDMODE_INVALID;
        strncpy(label, op, str - op);
        if (!is_label(label) || !is_mat(str))
            return ADDMODE_INVALID;
        return ADDMODE_MATRIX;
    }
}

/**
 * Copies the matrix args into arg1 and arg2
 * @param mat the matrix as string
 * @param arg1 the first arg initializer as string
 * @param arg2 the 2nd arg initializer as string
 */
void cpy_mat_vals(const char *mat, char *arg1, char *arg2) {
    char *op1, *op2;
    int i;
    char temp_mat[MAX_LINE] = "";
    strcpy(temp_mat, mat);
    op1 = strchr(mat, '[');
    for (i = 0; i < MAX_LINE && *(++op1) != ']'; i++)
        if (isspace(*op1))
            continue;
        else
            arg1[i] = *op1;
    arg1[i] = '\0';

    op2 = strchr(op1, '[');
    for (i = 0; i < MAX_LINE && *(++op2) != ']'; i++)
        if (isspace(*op2))
            continue;
        else
            arg2[i] = *op2;
    arg2[i] = '\0';
}