//
// Created by naortif on 8/9/17.
//

#include "helpers.h"
#include "assembler.h"

char *safe_strtok(char *str,char *delim){
    static char *str_cpy,*head = NULL,*cur = NULL;
    if(head == NULL)
        head  = str_cpy = malloc(sizeof(char)*MAX_LINE);
    if(str_cpy == NULL && head == NULL){
        fprintf(stderr,"Error allocating memory");
        exit(EXIT_FAILURE);
    }
    if(cur == NULL && str != NULL)
        str_cpy = strncpy(head,str,MAX_LINE);
    else
        str_cpy = NULL;
    cur = strtok(str_cpy,delim);
    return cur;
}