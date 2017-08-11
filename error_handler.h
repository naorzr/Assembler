//
// Created by naortif on 8/9/17.
//

#ifndef ENDPROJECT_ERROR_HANDLER_H
#define ENDPROJECT_ERROR_HANDLER_H

enum ErrorTypes{
    ERR_LABEL,
    ERR_EXPECTED_ARG,
    ERR_INV_WORD,
    ERR_STOP,
    ERR_LABEL_REDECLARED,
    NO_ERR_OCCURRED,
};
void update_err_log(enum ErrorTypes err,int lineNum,char *line);
#endif //ENDPROJECT_ERROR_HANDLER_H
