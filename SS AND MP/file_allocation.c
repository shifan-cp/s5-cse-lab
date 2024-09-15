#include <stdio.h>
#include <stdlib.h>

void sequential(){
    printf("\n---SEQUENTIAL---\n");
    int size; printf("Enter disk size: "); scanf("%d", &size); int disk[size];
    for (int i=0; i<size; i++){ disk[i] = -1; }
    int num; printf("Enter number of files: "); scanf("%d", &num);
    int start[num], len[num];
    for (int i=0; i<num; i++){
        start[i] = -1; len[i] = -1;
        int st = 0, l = 0, flag = 0; 
        printf("Enter starting block and length of file %d: ", i); scanf("%d %d", &st, &l);
        if (st<0 || st>size || (st+l)>size){ printf("Invalid range!\n"); } 
        else{
            for (int j=st; j<(st+l); j++){ if(disk[j]>-1){ flag = 1; } }
            if (flag==1){ printf("File cannot be allocated!\n"); } 
            else{
                start[i] = st; len[i] = l;
                for (int j=st; j<(st+l); j++){ disk[j] = i; }
            }
        }
    }
    printf("\nFile Table\nIndex\tStart\tLength\n");
    for (int i=0; i<num; i++){ 
        printf("%d\t", i);
        if(start[i]!=-1){ printf("%d\t%d\n", start[i], len[i]); } 
        else{ printf("NIL\tNIL\n"); }
    }
    printf("\nDisk representation\n");
    for (int i=0; i<size; i++){
        if (disk[i]==-1){ printf("- "); }
        else{ printf("%d ", disk[i]); }
    }
    printf("\n");
}

void indexed(){
    printf("\n---INDEXED---\n");
    int size; printf("Enter disk size: "); scanf("%d", &size); int disk[size], empty = size;
    for (int i=0; i<size; i++){ disk[i] = -1; }
    int num; printf("Enter number of files: "); scanf("%d", &num); int *table[num], len[num];
    for (int i=0; i<num; i++){
        table[i] = NULL; len[i] = -1;
        int l = 0; printf("Enter size of file %d: ", i); scanf("%d", &l);
        if (l>empty || l>size){ printf("File cannot be allocated!\n"); }
        else{
            int *temparr = (int *)malloc(l * sizeof(int)), ind = 0;
            len[i] = l;
            while(l>0){
                int rd_num = rand() % (size);
                if (disk[rd_num]>-1){ continue; }
                else{ 
                    disk[rd_num] = i; temparr[ind] = rd_num; ind++; l--; empty--;
                }
            }
            table[i] = temparr;
        }
    }
    printf("\nFile Table\nFile\tBlocks\n");
    for (int i=0; i<num; i++){
        printf("%d\t", i);
        if(table[i]!=NULL){ 
            for (int j = 0; j < len[i]; j++){ printf("%d, ", table[i][j]); }
        }
        else{ printf("NIL"); }
        printf("\n");
    }
    printf("\nDisk representation\n");
    for (int i=0; i<size; i++){
        if (disk[i]==-1){ printf("- "); }
        else{ printf("%d ", disk[i]); }
    }
    printf("\n");
}

void linked(){
    printf("\n---LINKED---\n");
    typedef struct block{ int file; int next; }block;
    int size; printf("Enter disk size: "); scanf("%d", &size); 
    struct block disk[size]; int empty = size;
    for (int i=0; i<size; i++){ disk[i].file = -1; }
    int num; printf("Enter number of files: "); scanf("%d", &num); int len[num], start[num], end[num];
    for (int i=0; i<num; i++){
        start[i] = -1; end[i] = -1; len[i] = -1;
        int l = 0, flag = 1; printf("Enter size of file %d: ", i); scanf("%d", &l);
        if (l>empty || l>size){ printf("File cannot be allocated!\n"); }
        else{
            int ptr = rand()%size; len[i] = l;
            while(l>0){
                if (disk[ptr].file>-1){ ptr = rand()%size; continue; }
                else{
                    if (flag == 1){ start[i] = ptr; flag = 0; }
                    int temp = ptr;
                    disk[ptr].file = i; l--; empty--;
                    if (l!=0){
                        while(disk[ptr].file>-1){ ptr = rand()%size; }
                        disk[temp].next = ptr;
                    }
                    else{
                        end[i] = ptr; disk[ptr].next = -1;
                    }
                }
            }
        }
    }
    printf("\nFile Table\nFile\tStart\tEnd\n");
    for (int i=0; i<num; i++){
        printf("%d\t", i);
        if(start[i]!=-1){
            printf("%d\t%d\n", start[i], end[i]);   
        }
        else{ printf("NIL\tNIL\n"); }
    }
    printf("\nDisk Representation\nBlock\tFile\tNext\n");
    for (int i=0; i<size; i++){
        printf("%d\t", i);
        if (disk[i].file==-1){ printf("-\t-\n"); }
        else{ 
            printf("%d\t", disk[i].file); 
            if (disk[i].next==-1){ printf("END\n"); }
            else{ printf("%d\n", disk[i].next); }
        }
    }
    printf("\n");
}

void main(){
    int ch=-1;
    while(ch!=4){
        printf("\n---MAIN MENU ---\n");
        printf("1. Sequential\n2. Indexed\n3. Linked\n4. Exit\nEnter your choice: ");
        scanf("%d", &ch);
        switch(ch){
            case 1: sequential(); break;
            case 2: indexed(); break;
            case 3: linked(); break;
            case 4: continue;
            default: printf("Invalid choice!\n");
        }
    }
}
