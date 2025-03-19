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
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define PROMPT "Nano Shell Prompt > "
#define INITIAL_VAR_CAPACITY 10



typedef struct { // Structure to store shell variables
    char *name;
    char *value;
} ShellVar;


typedef struct { //structure to with pointer to shell variables and count and capacity
    ShellVar *vars;
    int count;        /* Capacity: is the total available space in the vartable
                         Count:    is the accured space (busy space )      */
    int capacity;
} VarTable;






/***
 *** function prototypes  
 ***/

char** parse_input(char* input, int* arg_count); // imported from the Pico_Shell
int execute_builtin(char** args, int arg_count, VarTable *var_table); // imported from the Pico_Shell
int execute_external(char** args, VarTable *var_table); // imported from the Pico_Shell
void free_args(char** args, int arg_count); // imported from the Pico_Shell


int handle_assignment(char* input, VarTable *var_table); 
void init_var_table(VarTable *var_table); 
void add_var(VarTable *var_table, const char *name, const char *value);
char* get_var_value(VarTable *var_table, const char *name);
void free_var_table(VarTable *var_table);
char** substitute_variables(char** args, int arg_count, VarTable *var_table, int *new_count);
int is_valid_var_name(const char *name);
int export_var(VarTable *var_table, const char *name);




/***
 ***End of function prototypes  
 ***/







/**
 * Main function
 * 
 */

int main() {
    char input[MAX_INPUT_SIZE];
    char** args;
    int arg_count;
    int status = 1;
    VarTable var_table;
    
    // Initialize variable table
    init_var_table(&var_table);
    
    while (status) {
        // Display prompt
        printf("%s", PROMPT);
        fflush(stdout);
        
        // Read input
        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            printf("\nGood Bye\n");
            break;
        }
        
        // Remove trailing newline
        input[strcspn(input, "\n")] = '\0';
        
        // Skip empty lines
        if (strlen(input) == 0) {
            continue;
        }
        
        // Check if it's a variable assignment
        if (strchr(input, '=') != NULL) {
            if (handle_assignment(input, &var_table)) {
                continue;
            } else {
                printf("Invalid command\n");
                continue;
            }
        }
        
        // Parse input into arguments
        args = parse_input(input, &arg_count);
        if (args == NULL || arg_count == 0) {
            continue;
        }
        
        // Substitute variables in arguments
        int new_arg_count;
        char** substituted_args = substitute_variables(args, arg_count, &var_table, &new_arg_count);
        if (substituted_args == NULL) {
            free_args(args, arg_count);
            continue;
        }
        
        // Free original args if different from substituted
        if (substituted_args != args) {
            free_args(args, arg_count);
            args = substituted_args;
            arg_count = new_arg_count;
        }
        
        // Try to execute as built-in command
        if (!execute_builtin(args, arg_count, &var_table)) {
            // If not a built-in, try to execute as external command
            execute_external(args, &var_table);
        }
        
        // Free allocated memory
        free_args(args, arg_count);
    }
    
    // Free variable table
    free_var_table(&var_table);
    
    return 0;
}










/*
    Initilazing the var_table variables :))
*/
void init_var_table(VarTable *var_table) {
    var_table->vars = malloc(INITIAL_VAR_CAPACITY * sizeof(ShellVar));
    if (var_table->vars == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    var_table->count = 0;
    var_table->capacity = INITIAL_VAR_CAPACITY;
}


void add_var(VarTable *var_table, const char *name, const char *value) {
    for (int i = 0; i < var_table->count; i++) {    //checking if the value of the variable exists
        if (strcmp(var_table->vars[i].name, name) == 0) {   // if exists update the value of it
            

            free(var_table->vars[i].value); // Update existing variable
            var_table->vars[i].value = strdup(value);
            return;
        }
    }
    


    // Resize array if needed
    if (var_table->count >= var_table->capacity) {
        var_table->capacity *= 2;
        ShellVar *new_vars = realloc(var_table->vars, var_table->capacity * sizeof(ShellVar));
        if (new_vars == NULL) {
            perror("realloc failed");
            return;
        }
        var_table->vars = new_vars;
    }
    
    // Add new variable
    var_table->vars[var_table->count].name = strdup(name); // I have used strdup here for safety reasons like the memory mang. and memory safety :))
    var_table->vars[var_table->count].value = strdup(value);
    var_table->count++;
}




// Get value of a variable
char* get_var_value(VarTable *var_table, const char *name) {
    for (int i = 0; i < var_table->count; i++) {
        if (strcmp(var_table->vars[i].name, name) == 0) {
            return var_table->vars[i].value;
        }
    }
    return NULL;
}



// Free variable table
void free_var_table(VarTable *var_table) {
    for (int i = 0; i < var_table->count; i++) {
        free(var_table->vars[i].name);
        free(var_table->vars[i].value);
    }
    free(var_table->vars);
}



int is_valid_var_name(const char *name) { // Check if variable name is valid (alphanumeric and underscore)

    if (!isalpha(name[0]) && name[0] != '_') {
        return 0;
    }
    
    for (int i = 1; name[i] != '\0'; i++) {
        if (!isalnum(name[i]) && name[i] != '_') {
            return 0;
        }
    }
    
    return 1;
}






int handle_assignment(char* input, VarTable *var_table) {
    char *equals_pos = strchr(input, '=');     //used to locate a specific char in the string
    if (equals_pos == NULL) {
        return 0;
    }
    
    // Check if there's any command after the assignment
    char *space_pos = strchr(input, ' ');
    if (space_pos != NULL && space_pos < equals_pos) {
        return 0;  // Space before equals sign
    }
    
    if (space_pos != NULL) {
        return 0;  // Command after assignment
    }
    
    // Extract variable name and value
    int name_len = equals_pos - input; 
    char *name = malloc(name_len + 1); // creating a temp buffer to hold the varaible of the user
    if (name == NULL) {
        perror("malloc failed");
        return 0;
    }
    
    strncpy(name, input, name_len); // assiginin the value to it
    name[name_len] = '\0';
    
    // Check if variable name is valid
    if (!is_valid_var_name(name)) {
        free(name);
        return 0;
    }
    
    char *value = equals_pos + 1;
    
    // Add variable to table
    add_var(var_table, name, value);
    free(name);
    
    return 1;
}


// Export variable to environment
int export_var(VarTable *var_table, const char *name) {
    char *value = get_var_value(var_table, name);
    if (value == NULL) {
        return 0;
    }
    
    // Create environment variable string
    char *env_var = malloc(strlen(name) + strlen(value) + 2);
    if (env_var == NULL) {
        perror("malloc failed");
        return 0;
    }
    
    sprintf(env_var, "%s=%s", name, value);
    
    
    if (putenv(strdup(env_var)) != 0) {   // Add to environment ( the most important part )
        perror("putenv failed");
        free(env_var);
        return 0;
    }
    
    free(env_var);
    return 1;
}



// Substitute variables in arguments ( if someone wants to print avariable :)
char** substitute_variables(char** args, int arg_count, VarTable *var_table, int *new_count) {
    int has_substitution = 0;
    
    // Check if any argument contains a variable
    for (int i = 0; i < arg_count; i++) {
        if (strchr(args[i], '$') != NULL) {
            has_substitution = 1;
            break;
        }
    }
    
    if (!has_substitution) {
        *new_count = arg_count;
        return args;
    }
    
    // Create new argument array
    char **new_args = malloc((arg_count + 1) * sizeof(char*));
    if (new_args == NULL) {
        perror("malloc failed");
        *new_count = 0;
        return NULL;
    }
    
    *new_count = arg_count;
    
    // Process each argument
    for (int i = 0; i < arg_count; i++) {
        char *arg = args[i];
        char *dollar_pos = strchr(arg, '$');
        
        if (dollar_pos == NULL) {
            // No variable in this argument
            new_args[i] = strdup(arg);
            continue;
        }
        
        // Extract variable name
        char *var_name = dollar_pos + 1;
        char *end_pos = var_name;
        
        // Find end of variable name
        while (*end_pos && (isalnum(*end_pos) || *end_pos == '_')) {
            end_pos++;
        }
        
        int var_name_len = end_pos - var_name;
        char *extracted_name = malloc(var_name_len + 1);
        if (extracted_name == NULL) {
            perror("malloc failed");
            for (int j = 0; j < i; j++) {
                free(new_args[j]);
            }
            free(new_args);
            *new_count = 0;
            return NULL;
        }
        
        strncpy(extracted_name, var_name, var_name_len);
        extracted_name[var_name_len] = '\0';
        
        // Get variable value
        char *var_value = get_var_value(var_table, extracted_name);
        if (var_value == NULL) {
            var_value = "";  // Empty string if variable not found
        }
        
        // Calculate new argument length
        int prefix_len = dollar_pos - arg;
        int suffix_len = strlen(end_pos);
        int new_arg_len = prefix_len + strlen(var_value) + suffix_len + 1;
        
        // Create new argument with substitution
        char *new_arg = malloc(new_arg_len);
        if (new_arg == NULL) {
            perror("malloc failed");
            free(extracted_name);
            for (int j = 0; j < i; j++) {
                free(new_args[j]);
            }
            free(new_args);
            *new_count = 0;
            return NULL;
        }
        
        // Copy prefix
        strncpy(new_arg, arg, prefix_len);
        
        // Copy variable value
        strcpy(new_arg + prefix_len, var_value);
        
        // Copy suffix
        strcpy(new_arg + prefix_len + strlen(var_value), end_pos);
        
        new_args[i] = new_arg;
        free(extracted_name);
    }
    
    new_args[arg_count] = NULL;
    return new_args;
}




/**
The remaining function are imported from the PicoShell */

// Parse input string into array of arguments
char** parse_input(char* input, int* arg_count) {
    char* token;
    char** args = NULL;
    int count = 0;
    int capacity = 10; // Initial capacity
    
    // Allocate initial memory for arguments
    args = malloc(capacity * sizeof(char*));
    if (args == NULL) {
        perror("malloc failed");
        return NULL;
    }
    
    // Tokenize input
    token = strtok(input, " ");
    while (token != NULL) {
        // Resize array if needed
        if (count >= capacity) {
            capacity *= 2;
            char** new_args = realloc(args, capacity * sizeof(char*));
            if (new_args == NULL) {
                perror("realloc failed");
                free_args(args, count);
                return NULL;
            }
            args = new_args;
        }
        
        // Allocate memory for the token and copy it
        args[count] = strdup(token);
        if (args[count] == NULL) {
            perror("strdup failed");
            free_args(args, count);
            return NULL;
        }
        
        count++;
        token = strtok(NULL, " ");
    }
    
    // Add NULL terminator for execvp
    if (count >= capacity) {
        capacity++;
        char** new_args = realloc(args, capacity * sizeof(char*));
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

// Execute built-in commands
int execute_builtin(char** args, int arg_count, VarTable *var_table) {
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
    else if (strcmp(args[0], "export") == 0) {
        if (arg_count < 2) {
            fprintf(stderr, "export: missing variable name\n");
        } else {
            if (!export_var(var_table, args[1])) {
                fprintf(stderr, "export: variable '%s' not found\n", args[1]);
            }
        }
        return 1;
    }
    
    // Not a built-in command
    return 0;
}

// Execute external commands
int execute_external(char** args, VarTable *var_table) {
    pid_t pid = fork();
    
    if (pid == -1) {
        perror("fork");
        return 0;
    } 
    else if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("Command not found");
            exit(EXIT_FAILURE);
        }
    } 
    else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
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
