#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define PROMPT "Femto shell prompt > "

int main() {
    char input[MAX_INPUT_SIZE];
    char *command;
    char *args;
    
    while (1) {
        // Display prompt
        printf("%s", PROMPT);
        fflush(stdout);
        
        // Read input
        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            printf("\nGood Bye :)\n");
            break;
        }
        
        // Remove the '\n' character to avoid errors when parsing the input
        input[strcspn(input, "\n")] = '\0';
        
        
        if (strlen(input) == 0) { // Skip empty lines
            continue;
        }
        
        // Parse command
        command = strtok(input, " ");
        if (command == NULL) {
            continue;
        }
        
        
        if (strcmp(command, "exit") == 0) { // Handle exit command
            printf("Good Bye :)\n");
            break;
        }
        
        
        else if (strcmp(command, "echo") == 0) { // Handle echo command
            args = strtok(NULL, "");
            if (args != NULL) {
                printf("%s\n", args);
            } else {
                printf("\n");
            }
        }
        
        
        else { // Handle invalid commands
            printf("Invalid command\n");
        }
    }
    
    return 0;
}

