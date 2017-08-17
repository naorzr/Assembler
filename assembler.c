
#include "assembler.h"
#include "data_struct.h"
#include <string.h>
#include "error_handler.h"
#include "helpers.h"
#include "logger.h"

static int lineNum = 0;

char* get_line_content(FILE *inpf);
enum ErrorTypes parse_line(char *lineContent,int passage);


/* first_pass: takes in a file and parse all content. translating it to binary code and storing it in the relevant data structs
 * return: in case of an error returns the specific type of error */
enum ErrorTypes parse_file(FILE *inpf,int passage){
    struct {
        unsigned stop: 2;
    }flag = {FALSE};
    char *lineContent;
    enum ErrorTypes error;

    /* clears data/code and symbol table */
    if(passage == FIRST_PASS) {
        clear_data_stacks();
        free_symbtable();
    }
    if(passage == SECOND_PASS) {
        set_offset();
        rewind(inpf);
        clear_data_stacks();
    }

    while((lineContent = get_line_content(inpf)) != NULL)       /* parse line by line */
        if((error = parse_line(lineContent,passage)) != NO_ERR_OCCURRED) {
            update_err_log(error, lineNum,lineContent);
            flag.stop = TRUE;
        }
    return flag.stop == TRUE? ERR_STOP: NO_ERR_OCCURRED;    /* if an error has occurred tells the program to halt from going into second pass */
}

/* parse_line: parse each word in the line, storing it in the relavent data struct and converting to binary if needed
 * return: error type in case of an error */
enum ErrorTypes parse_line(char *lineContent,int passage) {
    unsigned dc,ic;
    char label[MAX_LINE] = "",cmd[MAX_LINE] = "",directive[MAX_LINE] = "",op1[MAX_LINE] = "",op2[MAX_LINE] = "";
    char *word;
    enum ErrorTypes error = NO_ERR_OCCURRED;
    const char comma[] = " ,\t";
    struct {
        unsigned label: 2;
    }flag = {0};
    dc = getDc(), ic = getIc();
    lineNum++;

    if(lineContent[0] == ';')       /* case of a comment line */
        return NO_ERR_OCCURRED;
    if((word = safe_strtok(lineContent, " \t")) == NULL)   /* case of an empty line */
        return NO_ERR_OCCURRED;

    if (LABEL_DEC(word)) {               /* case of label declaration */
        word[strlen(word)-1] = '\0';
        strcpy(label, word);
        if(!is_label(label))
            return ERR_LABEL;
        flag.label = TRUE;
        if((word = safe_strtok(NULL, " \t")) == NULL)
            return ERR_EXPECTED_ARG;
    }
    if(IS_DIRECTIVE(word)) {       /* case of a directive , in our project a directive is prefixed by a dot*/
        strcpy(directive, &word[1]);
        if (is_dsm(directive)) {      /* case it's a .data/.string/.mat directive */
            if (is_dsm(directive)) {
                if (flag.label && passage == FIRST_PASS)
                    error = updateSymbolTable(label, dc, RELOCATABLE, NONE_ENTRY, NOT_CMD2);
                if (word = safe_strtok(NULL, "")) {

                }
                strcpy(op1, word);
                updateData(directive, op1);
            }
        } else if (Is_External(directive)) {
            if ((word = safe_strtok(NULL, "")) == NULL)
                return ERR_EXPECTED_LABEL;
            strcpy(label, word);
            if(passage == FIRST_PASS)
                error = updateSymbolTable(label, EXTERNAL_ADDRESS, EXTERNAL, NONE_ENTRY, NOT_CMD2);

        } else if (Is_Entry(directive)) {
            if ((word = safe_strtok(NULL, "")) == NULL || !is_label(word))
                return ERR_EXPECTED_LABEL;
            strcpy(label, word);
            if(passage == SECOND_PASS)
                error = updateSymbolTable(label, dc, RELOCATABLE, ENTRY, NOT_CMD2);

        }
    }
    else if (isCmd(word)) {         /* case of a command*/
        strcpy(cmd, word);

        if(flag.label == TRUE && passage == FIRST_PASS)
            error = updateSymbolTable(label,ic,RELOCATABLE,NONE_ENTRY,CMD2);

        if (word = safe_strtok(NULL, comma))
            strcpy(op1, word);
        if (word = safe_strtok(NULL, comma))
            strcpy(op2, word);

        if (strcmp(op2, "") == 0)
            updateIc(cmd, "", op1, passage);
        else
            updateIc(cmd, op1, op2, passage);
    }
    if((word = safe_strtok(NULL,"")) != NULL)
        return ERR_INV_WORD;

    LOG_TRACE(LOG_DEBUG,
              "[DEBUG] line content: %s,\n\tlabel: %s,\n\tcommand: %s,\n\tdirective: %s\n\top1: %s,\n\top2: %s\n",
              lineContent==NULL?"":lineContent, label, cmd, directive,
              op1, op2);

    return error;

}

void export_assembly_files(char *outName){
    create_ob_file(outName);
    create_ext_file(outName);
    create_ent_file(outName);
}

/* get_line_content: handles each line of the given file
 * return: a pointer to the line */
char* get_line_content(FILE *inpf){
    static char lineContent[MAX_LINE];
    char *lineptr = lineContent;

    if(fgets(lineContent, MAX_LINE, inpf) != NULL) {
        Skip_Space(lineptr);
        if (lineptr[strlen(lineptr) - 1] == '\n')   /* removes the newline for simpler handling later with strtok */
            lineptr[strlen(lineptr) - 1] = '\0';
        return lineptr;
    }
    else
        return NULL;
}