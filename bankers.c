#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

int available[MAX_RESOURCES];  // Available resources
int max[MAX_PROCESSES][MAX_RESOURCES];  // Maximum demand of each process
int allocation[MAX_PROCESSES][MAX_RESOURCES];  // Resources currently allocated to each process
int need[MAX_PROCESSES][MAX_RESOURCES];  // Resources still needed by each process
int num_processes, num_resources;  // Number of processes and resources

// Function to initialize the Need matrix
void calculateNeed() {
    for (int i = 0; i < num_processes; i++) {
        for (int j = 0; j < num_resources; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
}

// Safety algorithm to check if the system is in a safe state
bool isSafe() {
    int work[MAX_RESOURCES]; // Temporary array for work
    bool finish[MAX_PROCESSES] = {false}; // To mark processes that have finished
    for (int i = 0; i < num_resources; i++) {
        work[i] = available[i];
    }

    int count = 0; // Count of finished processes
    while (count < num_processes) {
        bool found = false;
        for (int i = 0; i < num_processes; i++) {
            if (!finish[i]) {
                bool canProceed = true;
                for (int j = 0; j < num_resources; j++) {
                    if (need[i][j] > work[j]) {
                        canProceed = false;
                        break;
                    }
                }
                if (canProceed) {
                    for (int k = 0; k < num_resources; k++) {
                        work[k] += allocation[i][k];
                    }
                    finish[i] = true;
                    found = true;
                    count++;
                }
            }
        }

        if (!found) {
            // If no process could proceed, it's not safe
            return false;
        }
    }
    return true;
}

// Resource request algorithm to handle requests
bool resourceRequest(int process_id, int request[]) {
    // Check if the request is within the process's need
    for (int i = 0; i < num_resources; i++) {
        if (request[i] > need[process_id][i]) {
            printf("Error: Process has exceeded its maximum claim.\n");
            return false;
        }
    }

    // Check if resources are available
    for (int i = 0; i < num_resources; i++) {
        if (request[i] > available[i]) {
            printf("Resources not available. Process must wait.\n");
            return false;
        }
    }

    // Try allocating resources temporarily
    for (int i = 0; i < num_resources; i++) {
        available[i] -= request[i];
        allocation[process_id][i] += request[i];
        need[process_id][i] -= request[i];
    }

    // Check if the new allocation leads to a safe state
    if (isSafe()) {
        printf("Resources allocated successfully to Process %d.\n", process_id);
        return true;
    } else {
        // If not safe, roll back the allocation
        for (int i = 0; i < num_resources; i++) {
            available[i] += request[i];
            allocation[process_id][i] -= request[i];
            need[process_id][i] += request[i];
        }
        printf("Unsafe state detected! Request cannot be granted.\n");
        return false;
    }
}

int main() {
    printf("Enter the number of processes: ");
    scanf("%d", &num_processes);
    printf("Enter the number of resources: ");
    scanf("%d", &num_resources);

    // Input the available resources
    printf("Enter the available resources: \n");
    for (int i = 0; i < num_resources; i++) {
        scanf("%d", &available[i]);
    }

    // Input the maximum resources that each process can demand
    printf("Enter the maximum demand of each process: \n");
    for (int i = 0; i < num_processes; i++) {
        printf("For process %d: ", i);
        for (int j = 0; j < num_resources; j++) {
            scanf("%d", &max[i][j]);
        }
    }

    // Input the allocated resources to each process
    printf("Enter the allocated resources for each process: \n");
    for (int i = 0; i < num_processes; i++) {
        printf("For process %d: ", i);
        for (int j = 0; j < num_resources; j++) {
            scanf("%d", &allocation[i][j]);
        }
    }

    // Calculate the need matrix
    calculateNeed();

    // Display the current allocation, need, and available resources
    printf("Current resource allocation:\n");
    printf("Process\tAllocation\tMax\tNeed\n");
    for (int i = 0; i < num_processes; i++) {
        printf("P%d\t", i);
        for (int j = 0; j < num_resources; j++) {
            printf("%d ", allocation[i][j]);
        }
        printf("\t");
        for (int j = 0; j < num_resources; j++) {
            printf("%d ", max[i][j]);
        }
        printf("\t");
        for (int j = 0; j < num_resources; j++) {
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }

    // Run the resource request algorithm
    int process_id;
    int request[MAX_RESOURCES];
    printf("\nEnter the process ID making the request: ");
    scanf("%d", &process_id);
    printf("Enter the request for resources: \n");
    for (int i = 0; i < num_resources; i++) {
        scanf("%d", &request[i]);
    }

    // Process the request
    resourceRequest(process_id, request);

    return 0;
}
