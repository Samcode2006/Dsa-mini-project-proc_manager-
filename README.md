# DSA Mini Project - C Process Manager

<div align="center">

![C](https://img.shields.io/badge/Language-C-blue?style=for-the-badge&logo=c)
![Data Structure](https://img.shields.io/badge/Data%20Structure-Linked%20List-green?style=for-the-badge)
![Platform](https://img.shields.io/badge/Platform-Linux%2FUnix-orange?style=for-the-badge)
![License](https://img.shields.io/badge/License-Educational-yellow?style=for-the-badge)

A command-line process manager application written in C that demonstrates the practical application of **Linked Lists** data structure for managing system processes.

</div>

---

## Project Overview

This project implements a robust process management system using a **singly linked list** to store and manipulate running system processes. It provides functionalities to list, refresh, and terminate processes with an intuitive menu-driven interface and color-coded output for enhanced user experience.

## Data Structure

### ProcessNode Structure
```c
typedef struct ProcessNode
{
    int pid;                  // Process ID
    char name[256];           // Process Name
    struct ProcessNode *next; // Pointer to the next process
} ProcessNode;
```

Each node represents a single process with:
- **pid**: The unique process identifier
- **name**: The process/command name
- **next**: Pointer to the next node in the linked list

---

## Functions Explanation

### 1. **createNode(int pid, char *name)**
**Purpose**: Creates a new process node and allocates memory for it.

**Parameters**:
- `pid`: Process ID to be stored
- `name`: Process name/command to be stored

**Returns**: Pointer to newly created ProcessNode, or NULL if memory allocation fails

**Implementation Details**:
- Allocates memory using `malloc()`
- Initializes pid and name fields
- Sets next pointer to NULL
- Includes error handling for failed allocation

---

### 2. **appendProcess(ProcessNode **head, int pid, char *name)**
**Purpose**: Adds a new process node to the end of the linked list.

**Parameters**:
- `head`: Double pointer to the head of the list
- `pid`: Process ID to add
- `name`: Process name to add

**Returns**: void

**Implementation Details**:
- Creates a new node using `createNode()`
- If list is empty, sets the new node as head
- Otherwise, traverses to the last node and appends the new node
- Maintains the linked list structure

---

### 3. **freeList(ProcessNode *head)**
**Purpose**: Deallocates all memory used by the linked list.

**Parameters**:
- `head`: Pointer to the head of the list

**Returns**: void

**Implementation Details**:
- Iterates through all nodes
- Frees memory for each node individually
- Prevents memory leaks when exiting or clearing the list
- Essential for proper resource management

---

### 4. **deleteNode(ProcessNode **head, int pid)**
**Purpose**: Removes a specific node from the linked list by PID.

**Parameters**:
- `head`: Double pointer to the head of the list
- `pid`: Process ID to delete

**Returns**: 
- 1 if deletion was successful
- 0 if the PID was not found

**Implementation Details**:
- Handles special case: if head contains the target PID
- Traverses list to find the target node
- Unlinks the node from the list
- Frees the memory of the deleted node
- Updates pointers to maintain list integrity

---

### 5. **refreshProcessList(ProcessNode **head)**
**Purpose**: Reads current system processes and rebuilds the linked list.

**Parameters**:
- `head`: Double pointer to the head of the list

**Returns**: void

**Implementation Details**:
- Clears the existing list using `freeList()`
- Uses `popen()` to execute the system `ps` command
- Parses command output (PID and process name)
- Appends each process to the linked list
- Uses `sscanf()` to parse the output format
- Closes the pipe using `pclose()`
- Works on Unix/Linux systems

---

### 6. **printProcesses(ProcessNode *head)**
**Purpose**: Displays all processes in the linked list in a formatted table.

**Parameters**:
- `head`: Pointer to the head of the list

**Returns**: void

**Implementation Details**:
- Prints formatted header with column titles (PID, NAME)
- Iterates through the entire linked list
- Displays each process with color-coded formatting
- Uses ANSI color codes for enhanced readability
- Prints processes in the order they appear in the list

---

### 7. **killProcessOperation(ProcessNode **head)**
**Purpose**: Terminates a process and removes it from the linked list.

**Parameters**:
- `head`: Double pointer to the head of the list

**Returns**: void

**Implementation Details**:
- Prompts user to input a PID
- Uses `kill()` system call with SIGKILL signal to terminate the process
- If successful:
  - Removes the process from the linked list using `deleteNode()`
  - Displays success messages
- If unsuccessful:
  - Displays appropriate error messages
  - Uses `strerror()` to explain the error (permission denied, invalid PID, etc.)
- Includes error handling for invalid PIDs or insufficient permissions

---

### 8. **main()**
**Purpose**: Main program loop that provides the user interface.

**Returns**: 0 on successful exit

**Implementation Details**:
- Initializes head pointer to NULL
- Loads initial process list using `refreshProcessList()`
- Displays an interactive menu with options:
  - **Option 1**: Display all running processes
  - **Option 2**: Refresh process list from the operating system
  - **Option 3**: Kill a specific process by PID
  - **Option 4**: Exit the application
- Uses `switch-case` to handle user choices
- Continues running until user selects exit option
- Properly cleans up memory before exiting

## Key Features

✓ Linked List Implementation - Uses singly linked list for dynamic process storage  
✓ System Integration - Reads real-time processes from the operating system  
✓ Process Termination - Kill processes with proper signal handling  
✓ Color-Coded Output - ANSI color codes for better user experience  
✓ Memory Management - Proper allocation and deallocation of memory  
✓ Error Handling - Handles invalid inputs and system errors gracefully  
✓ Interactive Menu - User-friendly command-line interface  

---

## Color Codes Used

| Color | Usage |
|-------|-------|
| **RED** | Error messages and exit option |
| **GREEN** | Success messages and list options |
| **YELLOW** | Warning messages and user input prompts |
| **BLUE** | Headers and borders |
| **CYAN** | Main menu title and process information |

---

## Quick Start

### Prerequisites
- GCC compiler
- Linux/Unix system
- `ps` command available

### Compilation & Execution

```bash
# Compile the program
gcc proc_manager.c -o proc_manager

# Run normally
./proc_manager

# Run with sudo (recommended)
sudo ./proc_manager
```

---

## Usage Example

```
========================================
===   C PROCESS MANAGER v1.0        ===
========================================
1. List Processes
2. Refresh List (Read from OS)
3. Kill Process
4. Exit
Enter choice: 1

--- CURRENT RUNNING PROCESSES ---
PID        NAME
-----------------------------------
1          init
42         kthreadd
150        kworker/0:1
...
-----------------------------------

Enter choice: 3
Enter PID to kill: 150
Successfully sent KILL signal to PID 150.
Removed PID 150 from Process List.

Enter choice: 4
Goodbye!
```

---

## Algorithm Overview

### Linked List Operations Flow

```
[Head] -> [Node1] -> [Node2] -> [Node3] -> ... -> [NodeN] -> [NULL]
  |pid,name|  |pid,name|  |pid,name|             |pid,name|
```

### Process Management Workflow

1. **Initialization**: Load all system processes into linked list
2. **Display**: Traverse list and print all processes
3. **Search**: Traverse to find specific PID
4. **Delete**: Remove node and maintain list integrity
5. **Refresh**: Clear old list and rebuild from OS

---

## Error Handling

The program handles the following error scenarios:

| Error Type | Handling Method |
|-----------|-----------------|
| Memory Allocation Failure | Returns NULL, displays error message |
| Invalid PID Input | Shows error, uses strerror() |
| Permission Denied | Displays permission error, suggests sudo |
| Process Not Found | Notifies user, continues execution |
| Invalid Menu Choice | Shows "Invalid choice!" message |

---

## Data Structure Learning Outcomes

By studying this project, you will learn:

• Singly Linked List fundamentals  
• Dynamic Memory Allocation using malloc/free  
• Pointer Manipulation with double pointers  
• System Integration with OS commands  
• Signal Handling for process termination  
• Memory Leak Prevention techniques  
• Color-Coded Output using ANSI codes  

---

## File Structure

```
DSA-Mini-Project/
├── proc_manager.c          # Main C source code
├── proc_manager            # Compiled executable
├── README.md               # Documentation (this file)
└── .gitignore             # Git ignore rules
```

---

## License

This project is created for **educational purposes** as part of a DSA (Data Structures and Algorithms) mini project.

---

<div align="center">

Made with dedication by the Development Team

</div>
