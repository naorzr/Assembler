#include <string.h>
#include "error_handler.h"
#include "logger.h"

/**
 * Prints an error
 * @param err Error Type
 * @param lineNum Line Number
 * @param line Line Content
 */
void print_error(enum ErrorTypes err, int lineNum, char *line){
    char errTemplate[] = "Error: %s in line number %d, line content: %s\n";
    char errMsg[100];

    switch (err) {
        case ERR_LABEL:
            strcpy(errMsg, "Invalid label");
            break;
        case ERR_EXPECTED_ARG:
            strcpy(errMsg, "Expected argument");
            break;
        case ERR_EXPECTED_LABEL:
            strcpy(errMsg,"Expected a label");
            break;
        case ERR_INV_WORD:
            strcpy(errMsg, "Invalid word");
            break;
        case ERR_INV_DIRECTIVE:
            strcpy(errMsg, "Invalid directive");
            break;
        case ERR_STOP:
            strcpy(errMsg, "Expected argument");
            break;
        case ERR_LABEL_REDECLARED:
            strcpy(errMsg, "Duplicate label declaration");
            break;
        case E_INVALID_SRCOP_ADDMODE:
            strcpy(errMsg, "Invalid Source Operand");
            break;
        case E_INVALID_DESTOP_ADDMODE:
            strcpy(errMsg, "Invalid Destination Operand");
            break;
        case ERR_INV_DATA_PARAM:
            strcpy(errMsg, "Invalid data parameter");
            break;
        case ERR_INV_DATA_SIZE:
            strcpy(errMsg, "Data overflow (exceeds the 10 bit size)");
            break;
        case ERR_INV_STRING_PARAM:
            strcpy(errMsg, "Invalid string parameter");
            break;
        case ERR_INV_MAT_PARAM:
            strcpy(errMsg, "Invalid matrix parameter");
            break;
        case ERR_INV_MAT_INIT:
            strcpy(errMsg, "Invalid matrix initializer");
            break;
        case ERR_INV_MAT_PARAM_SIZE:
            strcpy(errMsg, "Matrix data overflow (exceeds the 10 bit size)");
            break;
        case ERR_EXSS_MAT_VAL:
            strcpy(errMsg, "Excessive matrix values");
            break;
        case E_MISS_OP:
            strcpy(errMsg, "Missing operand");
            break;
        case E_UNDECLARED_SYMBOL:
            strcpy(errMsg, "Undeclared symbol");
            break;
        default:
            strcpy(errMsg, "General error occurred");
            break;
    }

    LOG_TRACE(LOG_ERROR, errTemplate, errMsg, lineNum, line);
}