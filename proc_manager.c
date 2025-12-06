#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For sleep/system calls
#include <signal.h> // For killing processes
#include <errno.h>  // For error handling

// Define SIGKILL if not available (Windows compatibility)
#ifndef SIGKILL
#define SIGKILL 9
#endif

// Color codes
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define CYAN "\033[1;36m"
#define RESET "\033[0m"

// --- DATA STRUCTURE DEFINITION ---

// A Node representing a single process
typedef struct ProcessNode
{
    int pid;                  // Process ID
    char name[256];           // Process Name
    struct ProcessNode *next; // Pointer to the next process
} ProcessNode;

// --- LINKED LIST OPERATIONS ---

// Function to create a new node
ProcessNode *createNode(int pid, char *name)
{
    ProcessNode *newNode = (ProcessNode *)malloc(sizeof(ProcessNode));
    if (!newNode)
    {
        printf("Memory allocation failed.\n");
        return NULL;
    }
    newNode->pid = pid;
    strcpy(newNode->name, name);
    newNode->next = NULL;
    return newNode;
}

// Function to add a process to the end of the list
void appendProcess(ProcessNode **head, int pid, char *name)
{
    ProcessNode *newNode = createNode(pid, name);
    if (*head == NULL)
    {
        *head = newNode;
        return;
    }

    ProcessNode *temp = *head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = newNode;
}

// Function to free the entire list from memory
void freeList(ProcessNode *head)
{
    ProcessNode *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// Function to delete a specific node by PID (Logic update only)
int deleteNode(ProcessNode **head, int pid)
{
    ProcessNode *temp = *head, *prev = NULL;

    // If head holds the key to be deleted
    if (temp != NULL && temp->pid == pid)
    {
        *head = temp->next;
        free(temp);
        return 1; // Success
    }

    // Search for the key to be deleted
    while (temp != NULL && temp->pid != pid)
    {
        prev = temp;
        temp = temp->next;
    }

    // If key was not present in linked list
    if (temp == NULL)
        return 0;

    // Unlink the node from linked list
    prev->next = temp->next;
    free(temp);
    return 1; // Success
}

// --- SYSTEM OPERATIONS ---

// Function to read system processes and build the Linked List
void refreshProcessList(ProcessNode **head)
{
    // Clear existing list first
    freeList(*head);
    *head = NULL;

    printf(CYAN "Fetching system processes...\n" RESET);

    // Open a pipe to run the 'ps' command
    // -e: select all processes
    // -o pid,comm: output only PID and Command Name
    FILE *fp = popen("ps -e -o pid,comm", "r");
    if (fp == NULL)
    {
        printf(RED "Failed to run command.\n" RESET);
        return;
    }

    char line[1024];
    int pid;
    char name[256];

    // Skip the header line (PID COMMAND)
    fgets(line, sizeof(line), fp);

    // Read output line by line
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        // Parse PID and Name from string
        // %d reads integer, %s reads string (command)
        if (sscanf(line, "%d %s", &pid, name) == 2)
        {
            appendProcess(head, pid, name);
        }
    }

    pclose(fp);
}

void printProcesses(ProcessNode *head)
{
    printf(BLUE "\n--- CURRENT RUNNING PROCESSES ---\n" RESET);
    printf(CYAN "%-10s %-30s\n" RESET, "PID", "NAME");
    printf(BLUE "-----------------------------------\n" RESET);

    ProcessNode *current = head;
    while (current != NULL)
    {
        // Print every single process, no limits
        printf(GREEN "%-10d" RESET " %-30s\n", current->pid, current->name);
        current = current->next;
    }
    printf(BLUE "-----------------------------------\n" RESET);
}

void killProcessOperation(ProcessNode **head)
{
    int targetPID;
    printf(YELLOW "Enter PID to kill: " RESET);
    scanf("%d", &targetPID);
    getchar(); // Clear newline from input buffer

    // 1. Attempt to kill at System Level
    if (kill(targetPID, SIGKILL) == 0)
    {
        printf(GREEN "Successfully sent KILL signal to PID %d.\n" RESET, targetPID);

        // 2. Update Data Structure
        if (deleteNode(head, targetPID))
        {
            printf(GREEN "Removed PID %d from Process List.\n" RESET, targetPID);
        }
        else
        {
            printf(YELLOW "PID %d was killed but not found in current list.\n" RESET, targetPID);
        }
    }
    else
    {
        printf(RED "Error killing process (Permission denied or Invalid PID). Error: %s\n" RESET, strerror(errno));
    }
}

// --- MAIN MENU ---

int main()
{
    ProcessNode *head = NULL;
    int choice;

    // Load initial list
    refreshProcessList(&head);

    while (1)
    {
        printf(CYAN "\n========================================\n" RESET);
        printf(CYAN "===   C PROCESS MANAGER v1.0        ===\n" RESET);
        printf(CYAN "========================================\n" RESET);
        printf(GREEN "1. List Processes\n" RESET);
        printf(GREEN "2. Refresh List (Read from OS)\n" RESET);
        printf(GREEN "3. Kill Process\n" RESET);
        printf(RED "4. Exit\n" RESET);
        printf(YELLOW "Enter choice: " RESET);
        scanf("%d", &choice);
        getchar(); // Clear newline from input buffer

        switch (choice)
        {
        case 1:
            printProcesses(head);
            break;
        case 2:
            refreshProcessList(&head);
            printf(GREEN "List refreshed.\n" RESET);
            break;
        case 3:
            killProcessOperation(&head);
            break;
        case 4:
            freeList(head);
            printf(GREEN "\nGoodbye!\n" RESET);
            return 0;
        default:
            printf(RED "Invalid choice!\n" RESET);
        }
    }
    return 0;
}