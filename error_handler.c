//
// Created by naortif on 8/9/17.
//

#include <string.h>
#include "error_handler.h"
#include "logger.h"

/**
 * Prints an error
 * @param err Error Type
 * @param lineNum Line Number
 * @param line Line Content
 */
void update_err_log(enum ErrorTypes err, int lineNum, char *line){
    char errTemplate[] = "Error: %s in line number %d, line content: %s\n";
    char errMsg[100];

    switch (err) {
        case ERR_LABEL:
            strcpy(errMsg, "Invalid label");
            break;
        case ERR_EXPECTED_ARG:
            strcpy(errMsg, "Expected argument");
            break;
        case ERR_INV_WORD:
            strcpy(errMsg, "Invalid word");
            break;
        case ERR_STOP:
            strcpy(errMsg, "Expected argument");
            break;
        case ERR_LABEL_REDECLARED:
            strcpy(errMsg, "Duplicate label declaration");
            break;
        default:
            strcpy(errMsg, "General error occurred");
            break;
    }

    LOG_TRACE(LOG_ERROR, errTemplate, errMsg, lineNum, line);
}