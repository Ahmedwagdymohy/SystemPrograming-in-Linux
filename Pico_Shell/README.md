# Pico Shell

A lightweight shell implementation in C that supports both built-in commands and external program execution.

## Features

- **Built-in Commands**:
  - `echo`: Displays text following the command
  - `pwd`: Prints the current working directory
  - `cd`: Changes the current directory (supports `cd ~` for home directory)
  - `exit`: Terminates the shell

- **External Command Execution**: 
  - Executes any command available in the system PATH
  - Uses fork/exec system calls for process creation

- **Command Line Parsing**:
  - Parses user input into command and arguments
  - Handles multiple spaces as separators

## Technical Implementation

### Memory Management
- Uses dynamic memory allocation to handle variable argument counts
- Implements proper memory cleanup to prevent leaks
- Resizes argument arrays as needed using `realloc()`

### Process Management
- Creates child processes using `fork()`
- Executes commands with `execvp()` to automatically search PATH
- Parent process waits for child completion with `waitpid()`

### Input Parsing
- Tokenizes input using `strtok()` with space delimiter
- Creates NULL-terminated argument arrays for `execvp()`
- Uses `strdup()` to create independent copies of arguments

## Building and Running

Compile the shell with:
```bash
gcc -o pico_shell main.c
```

Run the shell:
```bash
./pico_shell
```

## Usage Examples

```
Pico shell> echo Hello World
Hello World

Pico shell> pwd
/home/user/Pico_Shell

Pico shell> cd ~
Pico shell> pwd
/home/user

Pico shell> ls -la
(displays directory contents)

Pico shell> exit
Good Bye
```

## Error Handling

- Checks for memory allocation failures
- Reports command execution errors
- Handles file system errors for built-in commands 