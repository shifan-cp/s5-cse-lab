
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH 20
FILE *inputFile, *optabFile, *symtabFile, *intermediateFile, *lengthFile;
int optabIndex = -1, symtabIndex = -1, operandValue, start, locctr, size;
char label[LENGTH], opcode[LENGTH], operand[LENGTH], temp1[LENGTH], temp2[LENGTH], temp3[LENGTH];

struct symtab_entry{
    char label[LENGTH];
    int address;
}symtab[LENGTH];

struct optab_entry{
    char opcode[LENGTH], hexcode[LENGTH];
}optab[LENGTH];

void read_OPTAB(){
    while(1){
        optabIndex++;
        fscanf(optabFile, "%s%s", optab[optabIndex].opcode, optab[optabIndex].hexcode);
        if(getc(optabFile) == EOF) break;
    }
}

void readNextLine(){
    strcpy(temp1,""), strcpy(temp2,""), strcpy(temp3,"");
    fscanf(inputFile, "%s", temp1);
    if(getc(inputFile) != '\n'){
        fscanf(inputFile, "%s", temp2);
        if(getc(inputFile) != '\n'){
            fscanf(inputFile, "%s", temp3);
        }
    }
    if (strcmp(temp3, "") != 0) {
        strcpy(label, temp1);
        strcpy(opcode, temp2);
        strcpy(operand, temp3);
    } 
    else if (strcmp(temp2, "") != 0) {
        strcpy(label, "");
        strcpy(opcode, temp1);
        strcpy(operand, temp2);
    } 
    else {
        strcpy(label, "");
        strcpy(opcode, temp1);
        strcpy(operand, "");
    }
}

int isDuplicateLabel(const char *label) {
    for (int i = 0; i <= symtabIndex; i++){
        if(strcmp(symtab[i].label, label) == 0)
            return 1;
    }
    return 0;
}

int findOpcode(const char *opcode) {
    for (int i = 0; i <= optabIndex; i++) {
        if (strcmp(optab[i].opcode, opcode) == 0)
            return i;
    }
    return -1;
}

void processOpcode(const char *opcode, const char *operand) {
    int index = findOpcode(opcode); 
    if (index != -1) {
        locctr += 0x3;
        size += 3;
    }
    else if (strcmp(opcode, "WORD") == 0) {
        locctr += 0x3;
        size += 3;
    } 
    else if (strcmp(opcode, "RESW") == 0) {
        locctr += 0x3 * atoi(operand);
    } 
    else if (strcmp(opcode, "RESB") == 0) {
        locctr += 0x1 * atoi(operand);
    } 
    else if (strcmp(opcode, "BYTE") == 0) {
 
        int length = (operand[0] == 'C' || operand[0] == 'c') ? strlen(operand) - 3 : (strlen(operand) - 3) / 2;
        locctr += length;
        size += length;
    }
}

int main(){
    inputFile = fopen("input.txt", "r");
    optabFile = fopen("optab.txt", "r");
    symtabFile = fopen("symtab.txt", "w");
    intermediateFile = fopen("intermediate.txt", "w");
    lengthFile = fopen("length.txt", "w");
    if (!inputFile || !optabFile|| !symtabFile || !intermediateFile || !lengthFile) {
        perror("Error opening one of the files!\n");
        exit(EXIT_FAILURE);
    }

    read_OPTAB();

    fscanf(inputFile, "%s%s%x", label, opcode, &operandValue);
 
    if(strcmp(opcode, "START") == 0){
        start = operandValue;
        locctr = start;
        fprintf(intermediateFile, "\t%s\t%s\t\t%x\n", label, opcode, operandValue);
        readNextLine();
    }
    else
        locctr = 0;
    while(strcmp(opcode, "END") != 0){
        if (strcmp(label, "") != 0)
            fprintf(intermediateFile, "%x\t%s\t%s\t%s\n", locctr, label, opcode, operand);
        else
            fprintf(intermediateFile, "%x\t%s\t%s\t%s\n", locctr, label, opcode, operand);
        if(strcmp(label, "") != 0){
            if(isDuplicateLabel(label)){
                printf("SYMTAB already contains label!\n");
                exit(EXIT_FAILURE);
            }
            symtabIndex++;
            strcpy(symtab[symtabIndex].label, label);
            symtab[symtabIndex].address = locctr;
        } 
        processOpcode(opcode, operand);
        readNextLine();
    }
    fprintf(intermediateFile, "\t%s\t%s\t%s\n", label, opcode, operand);
    for (int i = 0; i <= symtabIndex; i++){
        fprintf(symtabFile, "%s\t%x", symtab[i].label, symtab[i].address);
        if (i != symtabIndex)
            fprintf(symtabFile, "\n");
    }
    fprintf(lengthFile, "%x\n", locctr-start);
    fclose(inputFile);
    fclose(optabFile);
    fclose(symtabFile);
    fclose(intermediateFile);
    fclose(lengthFile);
    printf("Pass 1 completed successfully.\n");
}
