#include <stdio.h>

int main() {
    int i, j, k, np, nr;
    int alloc[50][50], max[50][50], avail[50];

    // Input number of processes and resources
    printf("Enter the number of processes: ");
    scanf("%d", &np);
    printf("Enter the number of resources: ");
    scanf("%d", &nr);

    // Input Allocation matrix
    printf("Enter the Allocation matrix:\n");
    for (i = 0; i < np; i++) {
        for (j = 0; j < nr; j++) {
            scanf("%d", &alloc[i][j]);
        }
    }

    // Input Max matrix
    printf("Enter the Max matrix:\n");
    for (i = 0; i < np; i++) {
        for (j = 0; j < nr; j++) {
            scanf("%d", &max[i][j]);
        }
    }

    // Input Available matrix
    printf("Enter the Available matrix:\n");
    for (i = 0; i < nr; i++) {
        scanf("%d", &avail[i]);
    }

    int need[np][nr], finish[np], safesequence[np], work[nr];

    // Calculate Need matrix
    for (i = 0; i < np; i++) {
        for (j = 0; j < nr; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }

    // Print Need matrix
    printf("\nNeed matrix\n");
    for (i = 0; i < np; i++) {
        for (j = 0; j < nr; j++) {
            printf("\t%d", need[i][j]);
        }
        printf("\n");
    }

    // Initialize Work and Finish arrays
    for (i = 0; i < nr; i++) {
        work[i] = avail[i];
    }
    for (i = 0; i < np; i++) {
        finish[i] = 0;
        safesequence[i] = -1;
    }

    // Safety algorithm
    int ind = 0;
    for (k = 0; k < np; k++) {
        for (i = 0; i < np; i++) {
            if (finish[i] == 0) {
                int flag = 0;
                for (j = 0; j < nr; j++) {
                    if (need[i][j] > work[j]) {
                        flag = 1;
                        break;
                    }
                }
                if (flag == 0) {
                    safesequence[ind++] = i;
                    for (j = 0; j < nr; j++) {
                        work[j] += alloc[i][j];
                    }
                    finish[i] = 1;
                }
            }
        }
    }

    // Check for deadlock
    int deadlock = 1;
    for (i = 0; i < np; i++) {
        if (finish[i] == 0) {
            deadlock = 0; // Deadlock detected
            break;
        }
    }

    // Output the result
    if (deadlock) {
        printf("\nNo deadlock detected. Following is the SAFE Sequence:\n");
        for (i = 0; i < ind; i++) {
            if (safesequence[i] != -1) {
                printf("P%d ", safesequence[i]);
            }
        }
    } else {
        printf("\nDeadlock detected. System is in an unsafe state.\n");
    }

    return 0;
}
