#include <string.h>

#ifndef ENDPROJECT_FILE_CONTENT_H
    #include "file_content.h"
#endif
#ifndef ENDPROJECT_LOGGER_H
    #include "logger.h"
#endif
#ifndef ENDPROJECT_CONTENT_VALIDATION_H
    #include "content_validation.h"
#endif
#ifndef ENDPROJECT_ASSEMBLER_H
    #include "assembler.h"
#endif
#ifndef ENDPROJECT_DATA_STRUCT_H
    #include "data_struct.h"
#endif


/**
 * Parse a single line into the file content structure (extract labels and commands and operands)
 * @param fileLine
 */
void parseLine(char *lineContent,int lineNum) {
    int dc,ic;
    char *word,operands;
    char comma[] = " ,\t";
    char label[MAX_LINE] = "",command[MAX_LINE] = "",directive[MAX_LINE] = "",op1[MAX_LINE] = "",op2[MAX_LINE] = "";
    unsigned wordSize = (unsigned int)strlen(word);
    dc = getDc(), ic = getIc();
    word = strtok(lineContent, " \t");
    struct {
        unsigned label: 2;
        unsigned dsm: 2;        /* data/string/mat */
    }flags = {0,0};

    if (word[wordSize - 1] == ':') { /* line begins with label */
        flags.label = ON;
        strcpy(label,word);
        //if(!validLabel(word))
          //  printerr(fileLine,fileLine->label);
        word = strtok(NULL, " \t");
    }

    if (word != NULL) {
//                TODO there might be more than two operands for a directive, therefor an array of operands(maxed to 40) should be created
//                TODO and filled using a while loop
                if(isDsm(word)){    /* case it's a .data/.string/.mat directive */
                    strcpy(directive, word);
                    if(flags.label) updateSymbolTable(label,dc,NONE,NO);

                    if (equals(directive, ".mat"))  /* fetch the matrix values */
                        word = strtok(NULL, " \t");
                    else
                        word = strtok(NULL,comma);

                    /* get operands */
                    if(word)
                        strcpy(op1, word);
                    word = strtok(NULL,comma);
                    if(word)
                        strcpy(op2,word);

                    updateDc(op1,op2);
                }
                else if(IS_EXTERNAL(word)){   /* case it's .extern/.entry directive */
                    strcpy(directive, word);

                    /* TODO check for erros for word = strtok */
                    word = strtok(NULL," \t");
                    strcpy(label,word);
                    if(equals(directive,".extern"))
                        updateSymbolTable(label,NOT_AVILABLE,EXTERN,NO);
                    else
                        updateSymbolTable(label,NOT_AVILABLE,ENTRY,NO);
                }
                else if(isCmd(word)) { /* normal command after label*/

                word = strtok(NULL, comma);
                if (word)
                    strcpy(op1, word);
                word = strtok(NULL, comma);
                if (word)
                    strcpy(op2, word);
                updateIcCounter(op1,op2,&ic);
                }
            } else {
                printerr(lineContent,word,lineNum);
            }




    LOG_TRACE(LOG_DEBUG,
              "[DEBUG] line content: %s,\n\tlabel: %s,\n\tcommand: %s,\n\top1: %s,\n\top2: %s\n",
              lineContent, label, command,
              op1, op2);

}



/* parse all the data in the input file into the fileContent structure */
void buildFileContent(FILE *inpf) {
    int lineNumber = 0;

    char lineContent[MAX_LINE];
    while (fgets(lineContent, sizeof(lineContent), inpf) != NULL) {
        lineNumber++;
        /* if line is comment or empty - skip */
        if (lineContent[0] == ';' || lineContent[0] == '\n') {
            continue;
        }

        int lineSize = strlen(lineContent);
        if (lineContent[lineSize - 1] == '\n')
            lineContent[lineSize - 1] = '\0';

        parseLine(lineContent,lineNumber);
    }


}



