//
// Created by naortif on 7/24/17.
//
#ifndef ENDPROJECT_CONTENT_VALIDATION_H
#define ENDPROJECT_CONTENT_VALIDATION_H

#endif //ENDPROJECT_CONTENT_VALIDATION_H


#ifndef ENDPROJECT_FILE_CONTENT_H
#include "file_content.h"
#endif

enum {FALSE,TRUE};

void printerr(FileLine *fileLine,char *str);

int validLabel(FileLine *fileLine);