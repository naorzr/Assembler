//
// Created by naortif on 8/9/17.
//

#include "error_handler.h"
#include "logger.h"

void update_err_log(enum ErrorTypes err,int lineNum,char *line){
    /* TODO: handle all error codes with proper error messages*/
    LOG_TRACE(LOG_ERROR, "Error type: %d was found in line number %d, line content: %s\n", err, lineNum, line);
}