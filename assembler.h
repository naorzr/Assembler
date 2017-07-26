
#ifndef ENDPROJECT_ASSEMBLER_H
#define ENDPROJECT_ASSEMBLER_H

#endif //ENDPROJECT_ASSEMBLER_H


#define MEMORY_STARTING_ADDRESS 100
#define DATA ".data"
#define MAT ".mat"
#define STRING ".string"
#define equals(str1, str2) strcmp(str1, str2) == 0

void firstIteration();
int getCommaCount(char*);
char *trimStringQuotes(char *);


int ic;
int dc;