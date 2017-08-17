//
// Created by naortif on 8/9/17.
//

#include "helpers.h"
#include "assembler.h"

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
void binToWierdFour(unsigned int bin,char *fourBaseWord,unsigned nmems){
    unsigned mask = 3;
    int i = 0;
    char *head = fourBaseWord;
    for(mask = 3;i < nmems;i++,bin>>=2,fourBaseWord++)
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
        }
    *(fourBaseWord) = '\0';
    reverse(head);
}


void *safe_malloc(int nmemb,size_t size){
    void *ptr = malloc(size*nmemb);
    if(ptr == NULL) {    /* TODO need to add code - error printing */
        fprintf(stderr,"Could not allocate memory");
        exit(EXIT_FAILURE);
    }


    return ptr;
}

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