/**
author: Ahmed Wagdy
date: 19/3/2025
description: A simple shell in C

 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_INPUT_SIZE 1024
#define PROMPT "Pico shell> "


/***
 *** Function prototypes
 ***/


/**
 * Parses a command line input string into an array of arguments
 * 
 * @param input     The input string to be parsed (null-terminated)
 * @param arg_count Pointer to an integer where the number of parsed arguments will be stored
 * 
 * @return         A dynamically allocated array of strings (char**) containing the parsed arguments,
 *                 with NULL as the last element. Returns NULL if memory allocation fails.
 *                 The caller is responsible for freeing the memory using free_args().
 * 
 * Example:
 *   Input: "ls -l /home"
 *   Result: ["ls", "-l", "/home", NULL]
 *   arg_count will be set to 3
 */
char** parse_input(char* input, int* arg_count);


/**
 * Executes a built-in command
 * 
 * @param args      Array of strings containing the command and its arguments
 * @param arg_count Number of arguments in the args array
 * 
 * @return         1 if the command was executed successfully, 0 otherwise
 * 
 * Example:
 *   Input: ["ls", "-l", "/home"]
 *   Result: Executes "ls -l /home"
 *   Returns 1
 */
int execute_builtin(char** args, int arg_count);

/**
 * Executes an external command
 * 
 * @param args Array of strings containing the command and its arguments
 * 
 * @return         1 if the command was executed successfully, 0 otherwise
 * 
 * Example:
 *   Input: ["ls", "-l", "/home"]
 *   Result: Executes "ls -l /home"
 *   Returns 1
 */
int execute_external(char** args);



/**
 * Frees the memory allocated for an array of strings
 * 
 * @param args       Array of strings to be freed
 * @param arg_count  Number of elements in the array
 * 
 * Example:
 *   Input: ["ls", "-l", "/home"]
 *   Result: Frees the memory allocated for the array
 */

void free_args(char** args, int arg_count);







/**
 * Main function
 * 
 */
int main() {
    char input[MAX_INPUT_SIZE];
    char** args;
    int arg_count;
    int status = 1;
    
    while (status) {
        
        printf("%s", PROMPT);  // Display prompt "Pico shell>"
        fflush(stdout);
        
        
        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) { // Read input
            printf("\nGood Bye\n");
            break;
        }
        
        
        input[strcspn(input, "\n")] = '\0'; // Remove the newline
        
        // Skip empty lines
        if (strlen(input) == 0) {
            continue;
        }
        

        
        args = parse_input(input, &arg_count); // Parse input into arguments
        if (args == NULL || arg_count == 0) {
            continue;
        }
        




        // Try to execute as built-in command
        if (!execute_builtin(args, arg_count)) {
            // If not a built-in, try to execute as external command
            execute_external(args);
        }
        
        // Free allocated memory
        free_args(args, arg_count);
    }
    
    return 0;
}






/****
 *** Functions implementation
 ***/







/*
 - Function to parse the input string into an array of arguments
 - It takes the input string and the number of arguments

*/

char** parse_input(char* input, int* arg_count) {
    char* token;
    char** args = NULL;
    int count = 0;
    int capacity = 10; // Initial capacity
    
    
    args = (char**)malloc(capacity * sizeof(char*)); // Allocate initial memory for arguments
    if (args == NULL) {
        perror("malloc failed");
        return NULL;
    }
    
    
    token = strtok(input, " "); // Tokenize for the input

    while (token != NULL) {
        // Resize array if needed
        if (count >= capacity) {
            capacity *= 2;
            char** new_args = (char**)realloc(args, capacity * sizeof(char*));
            if (new_args == NULL) {
                perror("realloc failed");
                free_args(args, count);
                return NULL;
            }
            args = new_args;
        }
        
        // Allocate memory for the token and copy it
        args[count] = strdup(token); //string duplicate
        if (args[count] == NULL) {
            perror("string duplicate failed");
            free_args(args, count);
            return NULL;
        }
        
        count++;
        token = strtok(NULL, " ");
    }
    
    // Add NULL terminator for execvp
    if (count >= capacity) {
        capacity++;
        char** new_args = (char**)realloc(args, capacity * sizeof(char*));
        if (new_args == NULL) {
            perror("realloc failed");
            free_args(args, count);
            return NULL;
        }
        args = new_args;
    }
    args[count] = NULL;
    
    *arg_count = count;
    return args;
}

// Execute built-in commands (exit, echo, pwd :)
int execute_builtin(char** args, int arg_count) {
    if (strcmp(args[0], "exit") == 0) {
        printf("Good Bye\n");
        exit(0);
    } 
    else if (strcmp(args[0], "echo") == 0) {
        for (int i = 1; i < arg_count; i++) {
            printf("%s", args[i]);
            if (i < arg_count - 1) {
                printf(" ");
            }
        }
        printf("\n");
        return 1;
    } 
    else if (strcmp(args[0], "pwd") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("getcwd");
        }
        return 1;
    } 
    else if (strcmp(args[0], "cd") == 0) {
        if (arg_count == 1 || strcmp(args[1], "~") == 0) {
            // Change to home directory
            char* home = getenv("HOME");
            if (home == NULL) {
                fprintf(stderr, "cd: HOME not set\n");
            } else if (chdir(home) != 0) {
                perror("cd");
            }
        } else if (chdir(args[1]) != 0) {
            perror("cd");
        }
        return 1;
    }
    
    // Not a built-in command
    return 0;
}

// Execute external commands
int execute_external(char** args) {
    pid_t pid = fork();
    
    if (pid == -1) {
        perror("fork");
        return 0;
    } 
    else if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) { //execvp — execute a program withoud the using of it's path
            perror("Command not found");
            exit(EXIT_FAILURE);
        }
    } 
    else {
        
        int status;
        waitpid(pid, &status, 0); //waitpid — wait for a child process to stop or terminate
        return 1;
    }
    
    return 0;
}

// Free allocated memory for arguments
void free_args(char** args, int arg_count) {
    for (int i = 0; i < arg_count; i++) {
        free(args[i]);
    }
    free(args);
}

