#ifndef ENDPROJECT_ERROR_HANDLER_H
#define ENDPROJECT_ERROR_HANDLER_H

/** Supported Error Types */
typedef enum {
    E_LABEL,        /* invalid label */
    E_EXPECTED_ARG,
    E_EXPECTED_LABEL,
    E_INV_WORD,
    E_INV_DATA_PARAM,
    E_INV_DATA_SIZE,
    E_INV_STRING_PARAM,
    E_INV_DIRECTIVE,
    E_INV_MAT_PARAM,
    E_INV_MAT_INIT,
    E_INV_MAT_PARAM_SIZE,
    E_INV_COMMA,
    E_EXSS_MAT_VAL,
    E_STOP,
    E_DATA_MISSING_ARG,
    E_LABEL_REDECLARED,
    E_UNDECLARED_SYMBOL,
    E_INVALID_ADDMODE,
    E_INVALID_SRCOP_ADDMODE,        /* invalid source operand address mode */
    E_INVALID_DESTOP_ADDMODE,       /* invalid destination operand address mode */
    E_MISS_OP,          /* missing operand */
    NO_ERR_OCCURRED,
    CODE_STACK_OVERFLOW,
    DATA_STACK_OVERFLOW
} ErrorTypes;

/**
 * Prints an error according to the error template and error code
 * @param err Error Type from list of supported errors
 * @param lineNum Line Number
 * @param line Line Content
 */
void print_error(ErrorTypes err, int lineNum, char *line);
#endif /*ENDPROJECT_ERROR_HANDLER_H*/
