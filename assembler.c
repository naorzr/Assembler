#include "assembler.h"
#include "data_struct.h"
#include "helpers.h"
#include "logger.h"

static int lineNum;
char* get_line_content(FILE *inpf);
ErrorTypes parse_line(char *lineContent,int passage);

/**
 * Takes in a file and parse all the content, translating it to binary code and storing it in the relevant data structs
 * @param inpf file input stream
 * @param passage Type of passage (first/second)
 * @return In case of an error returns the specific type of error
 */
ErrorTypes parse_file(FILE *inpf, int passage) {
    struct {
        unsigned stop: 2;
    } flag = {FALSE};
    char *lineContent;
    ErrorTypes error;
    lineNum = 0;
    /* clears data/code and symbol table */
    if (passage == FIRST_PASS) {
        clear_code_arr();
        clear_data_stacks();
        free_symbtable();
        free_ext_ref();
    }

    if (passage == SECOND_PASS) {
        set_offset();
        rewind(inpf); /* go back to the beginning of the file in the 2nd pass */
        clear_data_stacks();
    }

    while ((lineContent = get_line_content(inpf)) != NULL) {      /* parse line by line */
        lineNum++;
        if ((error = parse_line(lineContent, passage)) != NO_ERR_OCCURRED) {
            print_error(error, lineNum, lineContent);
            flag.stop = TRUE;
        }
    }
    return flag.stop == TRUE? E_STOP: NO_ERR_OCCURRED;    /* if an error has occurred tells the program to halt from going into second pass */
}

/**
 * Parse each word in the line, storing it in the relevant data struct and converting to binary if needed
 * @param lineContent line to parse
 * @param passage current passage of the program on the current line(first pass or second)
 * @return error type in case of an error
 */
ErrorTypes parse_line(char *lineContent, int passage) {
    int dc, ic;
    char label[MAX_LINE] = "", cmd[MAX_LINE] = "", directive[MAX_LINE] = "", op1[MAX_LINE] = "", op2[MAX_LINE] = "";
    char *word;
    ErrorTypes errCode = NO_ERR_OCCURRED;
    const char comma[] = " ,\t";
    struct {
        unsigned label: 2;
    } flag = {0};
    dc = get_dc();
    ic = get_ic();

    /* case of a comment line */
    if (lineContent[0] == ';')
        return NO_ERR_OCCURRED;

    /* case of an empty line */
    if ((word = safe_strtok(lineContent, " \t")) == NULL)
        return NO_ERR_OCCURRED;
    if (!valid_commas(lineContent)) /* validate that there are no extra commas */
        return E_INV_EXTRA_COMMA;

    /* case of label declaration */
    if (LABEL_DEC(word)) {
        word[strlen(word)-1] = '\0';
        strcpy(label, word);
        if (!is_label(label))
            return E_LABEL;
        flag.label = TRUE;
        if ((word = safe_strtok(NULL, " \t")) == NULL)
            return E_EXPECTED_ARG;
    }
    /* case of a directive, in our project a directive is prefixed by a dot*/
    if (Is_Directive(word)) {
        strcpy(directive, &word[1]);

        /* in case it's a data/string/mat - (dsm) directive */
        if (is_dsm(directive)) {
            if (flag.label && passage == FIRST_PASS)    /* if there is a label declaration insert label into symbol table */
                errCode = insert_to_symtab(label, dc, RELOCATABLE, NONE_ENTRY, NOT_CMD2);

            if (errCode != NO_ERR_OCCURRED)
                return errCode;    /* could not insert to symbol table due to some error */

            if ((word = safe_strtok(NULL, "")) == NULL)
                return E_DATA_MISSING_ARG;  /* missing argument after .data declaration */

            strcpy(op1, word);
            errCode = update_data(directive, op1);
        }
        /* case of external declaration */
        else if (Is_External(directive)) {

            if ((word = safe_strtok(NULL, " \t")) == NULL || !is_label(word))
                return E_EXPECTED_LABEL;  /* missing/invalid label after external directive */

            strcpy(label, word);

            if (passage == FIRST_PASS)
                errCode = insert_to_symtab(label, EXTERNAL_ADDRESS, EXTERNAL, NONE_ENTRY, NOT_CMD2);
        }
        /* in case it's an entry declaration */
        else if (Is_Entry(directive)) {

            if ((word = safe_strtok(NULL, " \t")) == NULL || !is_label(word))
                return E_EXPECTED_LABEL; /* missing/invalid label after entry directive */

            strcpy(label, word);
            if (passage == SECOND_PASS)
                errCode = update_sym_table(label, RELOCATABLE, ENTRY);
        }
    }
    /* case of a command*/
    else if (is_cmd(word)) {
        strcpy(cmd, word);

        /* in case of label preceding a command, update the symbol table */
        if (flag.label == TRUE && passage == FIRST_PASS)
            errCode = insert_to_symtab(label, ic, RELOCATABLE, NONE_ENTRY, CMD2);

        if (errCode != NO_ERR_OCCURRED)
            return errCode; /* could not update symbol table due to some error */

        /* Fetch first op */
        if ((word = safe_strtok(NULL, comma)) != NULL)
            strcpy(op1, word);
        /* Fetch 2nd op */
        if ((word = safe_strtok(NULL, comma)) != NULL)
            strcpy(op2, word);

        if (strcmp(op2, "") == 0)
            errCode = update_code(cmd, "", op1, passage);
        else
            errCode = update_code(cmd, op1, op2, passage);
    }
    /* check that there are no extra words exceeding the line*/
    if (errCode == NO_ERR_OCCURRED && (safe_strtok(NULL, "")) != NULL)
        return E_INV_WORD;

    return errCode;
}

/**
 * Create assembly output files (.ext, .ob, .ent)
 * @param outName file name to output
 */
void export_assembly_files(char *outName){
    create_ob_file(outName);
    create_ext_file(outName);
    create_ent_file(outName);
    test(outName);
}

/**
 * Handles each line of the given file
 * @param inpf input file to process
 * @return a pointer to the line
 */
char* get_line_content(FILE *inpf){
    static char lineContent[MAX_LINE] = "";
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