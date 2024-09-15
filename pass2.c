#include <stdio.h>
#include <stdlib.h>
#include <string.h>




#define LENGTH 20
FILE *intermediateFile, *symtabFile, *optabFile, *lengthFile, *assemblyListingFile, *objcodeFile;
int optabIndex = -1, symtabIndex = -1;
char address[LENGTH], label[LENGTH], opcode[LENGTH], operand[LENGTH], length[LENGTH], size[LENGTH], startAddress[LENGTH], temp1[LENGTH], temp2[LENGTH], temp3[LENGTH], temp4[LENGTH], value[LENGTH];


struct symtab_entry{
    char label[LENGTH], address[LENGTH];
}symtab[LENGTH];

struct optab_entry{
    char opcode[LENGTH], hexcode[LENGTH];
}optab[LENGTH];


void read_optab(){
    while(1){
        optabIndex++;
        fscanf(optabFile, "%s%s", optab[optabIndex].opcode, optab[optabIndex].hexcode);
        if(getc(optabFile) == EOF) break;
    }
}


void read_symtab(){
    while(1){
        symtabIndex++;
        fscanf(symtabFile, "%s%s", symtab[symtabIndex].label, symtab[symtabIndex].address);
        if(getc(symtabFile) == EOF) break;
    }
}


void readNextLine(){
    strcpy(temp1,""), strcpy(temp2,""), strcpy(temp3,""); strcpy(temp4,"");
    fscanf(intermediateFile, "%s", temp1);
    if(getc(intermediateFile) != '\n'){
        fscanf(intermediateFile, "%s", temp2);
        if(getc(intermediateFile) != '\n'){
            fscanf(intermediateFile, "%s", temp3);
            if(getc(intermediateFile) != '\n'){
                fscanf(intermediateFile, "%s", temp4);
            }
        }
    }
    if (strcmp(temp4, "") != 0) {
        strcpy(address, temp1);
        strcpy(label, temp2);
        strcpy(opcode, temp3);
        strcpy(operand, temp4);
    } 
    else if (strcmp(temp3, "") != 0) {
        strcpy(address, temp1);
        strcpy(label, "");
        strcpy(opcode, temp2);
        strcpy(operand, temp3);
    }
    else if (strcmp(temp1, "END") == 0){
        strcpy(address, "");
        strcpy(label, "");
        strcpy(opcode, temp1);
        strcpy(operand, temp2);
    }
    else {
        strcpy(address, temp1);
        strcpy(label, "");
        strcpy(opcode, temp2);
        strcpy(operand, "");
    }
}

int main(){
    intermediateFile = fopen("intermediate.txt", "r");
    symtabFile = fopen("symtab.txt", "r");
    optabFile = fopen("optab.txt", "r");
    lengthFile = fopen("length.txt", "r");
    assemblyListingFile = fopen("assemblyListing.txt", "w");
    objcodeFile = fopen("objcode.txt", "w");
    fscanf(lengthFile, "%s%s", length, size);
    read_optab();
    read_symtab();
    //read first line from intermediate file
    fscanf(intermediateFile, "%s%s%s", label, opcode, operand);
    strcpy(startAddress, operand);
    if (strcmp(opcode, "START") == 0){
        fprintf(assemblyListingFile, "\t%s\t%s\t%s\n", label, opcode, operand);
        fprintf(objcodeFile, "H^%s^", label);
        for (int i = 0; i < (6 - strlen(operand)); i++){
            fprintf(objcodeFile, "0");
        }
        fprintf(objcodeFile, "%s^", operand);
        for (int i = 0; i < (6 - strlen(length)); i++){
            fprintf(objcodeFile, "0");
        }
        fprintf(objcodeFile, "%s\n", length);
        fprintf(objcodeFile, "T^");
        for (int i = 0; i < (6 - strlen(operand)); i++){
            fprintf(objcodeFile, "0");
        }
        fprintf(objcodeFile, "%s^%x", operand, atoi(size));
        readNextLine();
    }
    while(strcmp(opcode, "END") != 0){
        if (strcmp(opcode, "BYTE") == 0){
            fprintf(assemblyListingFile, "%s\t%s\t%s\t%s\t", address, label, opcode, operand);
            sprintf(value, "%x", operand[2]);
            fprintf(assemblyListingFile, "%s\n", value);
            fprintf(objcodeFile, "^%s", value);
        }
        else if (strcmp(opcode, "WORD") == 0){
            sprintf(value, "%x", atoi(operand));
            fprintf(assemblyListingFile, "%s\t%s\t%s\t%s\t00000%s\n", address, label, opcode, operand, value);
            fprintf(objcodeFile, "^00000%s", value);
        }
        else if ((strcmp(opcode, "RESB") == 0) || (strcmp(opcode, "RESW") == 0)){
            fprintf(assemblyListingFile, "%s\t%s\t%s\t%s\n", address, label, opcode, operand);
        }
        else{
            int i = 0, j = 0;
            while (strcmp(opcode, optab[j].opcode) != 0) j++;
            while (strcmp(operand, symtab[i].label) != 0) i++;
            fprintf(assemblyListingFile, "%s\t%s\t%s\t%s\t%s%s\n", address, label, opcode, operand, optab[j].hexcode, symtab[i].address);
            fprintf(objcodeFile, "^%s%s", optab[j].hexcode, symtab[i].address);
        }
        readNextLine();
    }
    fprintf(assemblyListingFile, "\t%s\t%s\t%s", label, opcode, operand);
    fprintf(objcodeFile, "\nE^");
    for (int i = 0; i < (6 - strlen(startAddress)); i++){
        fprintf(objcodeFile, "0");
    }
    fprintf(objcodeFile, "%s", startAddress);
    fclose(assemblyListingFile);
    fclose(optabFile);
    fclose(symtabFile);
    fclose(intermediateFile);
    fclose(lengthFile);
    fclose(objcodeFile);
    printf("Pass 2 completed.\n");
}
