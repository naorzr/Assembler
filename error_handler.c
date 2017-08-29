#include <string.h>
#include "error_handler.h"
#include "logger.h"

/**
 * Prints an error according to the error template and error code
 * @param err Error Type from list of supported errors
 * @param lineNum Line Number
 * @param line Line Content
 */
void print_error(ErrorTypes err, int lineNum, char *line){
    char errTemplate[] = "Error in line %d: %s, line content: %s\n";
    char errMsg[100];

    switch (err) {
        case E_LABEL:
            strcpy(errMsg, "Invalid label");
            break;
        case E_EXPECTED_ARG:
            strcpy(errMsg, "Expected argument");
            break;
        case E_EXPECTED_LABEL:
            strcpy(errMsg,"Expected a label");
            break;
        case E_INV_WORD:
            strcpy(errMsg, "Invalid word");
            break;
        case E_INV_COMMA:
            strcpy(errMsg, "Invalid comma found");
            break;
        case E_INV_DIRECTIVE:
            strcpy(errMsg, "Invalid directive");
            break;
        case E_STOP:
            strcpy(errMsg, "Expected argument");
            break;
        case E_LABEL_REDECLARED:
            strcpy(errMsg, "Duplicate label declaration");
            break;
        case E_INVALID_SRCOP_ADDMODE:
            strcpy(errMsg, "Invalid Source Operand");
            break;
        case E_INVALID_DESTOP_ADDMODE:
            strcpy(errMsg, "Invalid Destination Operand");
            break;
        case E_INV_DATA_PARAM:
            strcpy(errMsg, "Invalid data parameter");
            break;
        case E_INV_DATA_SIZE:
            strcpy(errMsg, "Data overflow (exceeds the 10 bit size)");
            break;
        case E_INV_STRING_PARAM:
            strcpy(errMsg, "Invalid string parameter");
            break;
        case E_INV_MAT_PARAM:
            strcpy(errMsg, "Invalid matrix parameter");
            break;
        case E_INV_MAT_INIT:
            strcpy(errMsg, "Invalid matrix initializer");
            break;
        case E_INV_MAT_PARAM_SIZE:
            strcpy(errMsg, "Matrix data overflow (exceeds the 10 bit size)");
            break;
        case E_EXSS_MAT_VAL:
            strcpy(errMsg, "Excessive matrix values");
            break;
        case E_MISS_OP:
            strcpy(errMsg, "Missing operand");
            break;
        case E_UNDECLARED_SYMBOL:
            strcpy(errMsg, "Undeclared symbol");
            break;
        case E_DATA_MISSING_ARG:
            strcpy(errMsg, "Data missing argument");
            break;
        default:
            strcpy(errMsg, "General error occurred");
            break;
    }

    log_trace(LOG_ERROR, errTemplate, lineNum, errMsg, line);
}