#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct holes {
    int lastIndex;
    int index[5];
    int sizes[5];
};

struct process {
    int arr_size;
    int* allocated;
    int* memory;
};


void firstFit(struct holes* data, struct process* processes) {
    for (int i=0; i<processes->arr_size;i++) {
        int processSize = processes->memory[i];
        for (int j=0;j<5;j++) {
            if (data->sizes[j] >= processSize)
            {
                data->sizes[j] -= processSize;
                processes->allocated[i] = data->index[j];
                data->index[j] = data->lastIndex+1;
                data->lastIndex++;
                break;
            }
        }
    }
}


void bestFit(struct holes* data, struct process* processes) {

    for (int i=0; i<(processes->arr_size);i++) {
        int index = -1;
        int processSize = processes->memory[i];
        int bestSize = INT_MAX;

        for (int j=0;j<5;j++) {
            if (data->sizes[j] >= processSize && data->sizes[j] < bestSize) {
                index = j;
                bestSize = data->sizes[j];
            }
        }

        if (index != -1) {
            data->sizes[index] -= processSize;
            processes->allocated[i] = data->index[index];
            data->index[index] = data->lastIndex + 1;
            data->lastIndex++;
        }
    } 
}

int main(void) {
    //Holes Information
    struct holes holesData;

    holesData.lastIndex = 4;

    holesData.index[0] = 0;
    holesData.index[1] = 1;
    holesData.index[2] = 2;
    holesData.index[3] = 3;
    holesData.index[4] = 4;

    holesData.sizes[0] = 100;
    holesData.sizes[1] = 500;
    holesData.sizes[2] = 200;
    holesData.sizes[3] = 300;
    holesData.sizes[4] = 600;

    //Process Information
    // struct process processes;
    // processes.memory[0] = 212;
    // processes.memory[1] = 400;
    // processes.memory[2] = 101;
    // processes.memory[3] = 530;
    // processes.memory[4] = 200;

    //Process Information
    struct process processes;
    printf("Enter no. of processes: ");
    scanf("%d", &processes.arr_size);
    processes.memory = (int *) malloc(sizeof(int) * processes.arr_size);
    processes.allocated = (int *) malloc(sizeof(int) * processes.arr_size);

    //Default allocation -1
    for (int i=0;i<processes.arr_size;i++) {
        processes.allocated[i] = -1;
    }

    for (int i=0; i<processes.arr_size; i++) {
        printf("Enter memory info about process %d: ", i);
        scanf("%d", &processes.memory[i]);
    }

    // firstFit(&holesData, &processes);
    bestFit(&holesData, &processes);

    // Check final hole sizes
    // for (int i=0;i<5;i++) {
    //     printf("Holes %d: %d\n" , i, holesData.sizes[i]);
    // }

    int check = 1;
    if (check == 0) {
        for (int i=0; i<processes.arr_size;i++) {
            printf("Final allocation of process %d in First Fit: %d\n", i, processes.allocated[i]);
        }
    } else {
        for (int i=0; i<processes.arr_size;i++) {
            printf("Final allocation of process %d in Best Fit: %d\n", i, processes.allocated[i]);
        }
    }
    return 0;
}