
#include "assembler.h"
#include "data_struct.h"
#include "string.h"
#include "error_handler.h"
#include "helpers.h"
#include "logger.h"

static int lineNum = 1;

char* get_line_content(FILE *inpf);
enum ErrorTypes parse_line(char *lineContent);



enum ErrorTypes first_pass(FILE *inpf){
    struct {
        unsigned stop: 2;
    }flag = {FALSE};
    char *lineContent;
    enum ErrorTypes error;
    while((lineContent = get_line_content(inpf)) != NULL)
        if((error = parse_line(lineContent)) != NO_ERR_OCCURRED) {
            update_err_log(error, lineNum,lineContent);
            flag.stop = TRUE;
        }
    return flag.stop == TRUE? ERR_STOP: NO_ERR_OCCURRED;
}

enum ErrorTypes parse_line(char *lineContent) {
    unsigned dc,ic;
    char label[MAX_LINE] = "",cmd[MAX_LINE] = "",directive[MAX_LINE] = "",op1[MAX_LINE] = "",op2[MAX_LINE] = "";
    char *word;
    const char comma[] = " ,\t";
    struct {
        unsigned label: 2;
    }flag = {0};
    dc = getDc(), ic = getIc();

    if(lineContent[0] == ';')       /* case of a comment line */
        return NO_ERR_OCCURRED;
    lineNum++;
    if((word = safe_strtok(lineContent, " \t")) == NULL)   /* case of an empty line */
        return NO_ERR_OCCURRED;

    if (LABEL_DEC(word)) {               /* case of label declaration */
        strcpy(label, word);
        if(!is_label(label))
            return ERR_LABEL;
        flag.label = TRUE;
        if((word = safe_strtok(NULL, " \t")) == NULL)
            return ERR_EXPECTED_SENTENCE;
    }

    if (DIRECTIVE_DEC(word)) {
        strcpy(directive, &word[1]);
        if (is_dsm(directive)) {
            if (flag.label) updateSymbolTable(label, dc, DSM, NOT_CMD);
            if (word = safe_strtok(NULL, "")) {

            }
            strcpy(op1, word);
            updateData(directive, op1);
            /*TODO: need to check what to actually do with external or entry labels */
        } else if ((Is_External(directive) || Is_Entry(directive))) {
            if ((word = safe_strtok(NULL, " \t")) == NULL)
                return ERR_EXPECTED_SENTENCE;
            strcpy(label, word);
            updateSymbolTable(label, EXTERNAL_ADDRESS, strcmp(directive, "extern") == 0 ? EXTERN : ENTRY, NOT_CMD);
        }
    }
    else if (isCmd(word)) { /* normal command after label*/
        strcpy(cmd, word);

        if (word = safe_strtok(NULL, comma))
            strcpy(op1, word);
        if (word = safe_strtok(NULL, comma))
            strcpy(op2, word);

        if (strcmp(op2, "") == 0)
            updateIc(cmd, "", op1, FIRST_PASS);
        else
            updateIc(cmd, op1, op2, FIRST_PASS);
    }
    if((word = safe_strtok(NULL,"")) != NULL)
        return ERR_INV_WORD;

    LOG_TRACE(LOG_DEBUG,
              "[DEBUG] line content: %s,\n\tlabel: %s,\n\tcommand: %s,\n\tdirective: %s\n\top1: %s,\n\top2: %s\n",
              lineContent==NULL?"":lineContent, label, cmd, directive,
              op1, op2);



}

char* get_line_content(FILE *inpf){
    static char lineContent[MAX_LINE];
    char *lineptr = lineContent;
    lineNum++;
    if(fgets(lineContent, MAX_LINE, inpf) != NULL) {
        Skip_Space(lineptr);
        if (lineptr[strlen(lineptr) - 1] == '\n')
            lineptr[strlen(lineptr) - 1] = '\0';
        return lineptr;
    }
    else
        return NULL;
}