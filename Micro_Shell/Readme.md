

# Micro Shell

The Micro Shell is an advanced shell implementation in C that extends the functionality of the Nano Shell by adding support for I/O redirection. This shell allows users to execute commands with input, output, and error redirection, in addition to supporting built-in commands and variable management.

## Features

- **Built-in Commands**:
  - `echo`: Displays text following the command
  - `pwd`: Prints the current working directory
  - `cd`: Changes the current directory (supports `cd ~` for home directory)
  - `exit`: Terminates the shell
  - `export`: Adds shell variables to environment variables

- **Variable Management**:
  - Define variables using `variable=value` syntax
  - Access variable values using `$variable` notation
  - Export variables to environment with `export variable`

- **I/O Redirection**:
  - `<`: Redirects input from a file
  - `>`: Redirects output to a file
  - `2>`: Redirects error output to a file
  - Supports multiple redirections in a single command

- **External Command Execution**: 
  - Executes any command available in the system PATH
  - Uses fork/exec system calls for process creation
  - Inherits environment variables

## Technical Implementation

### Variable Storage Structure

The shell uses a dynamic data structure to store variables:

```
VarTable
├── vars ──────┬─► ShellVar[0] { name: "x", value: "5" }
├── count: 2   ├─► ShellVar[1] { name: "folder", value: "home" }
└── capacity: 4 ├─► [empty]
                └─► [empty]
```

- `VarTable` contains a dynamically allocated array of `ShellVar` structures
- `count` tracks the number of variables currently stored
- `capacity` represents the total space available in the array
- The array automatically resizes when needed

### Memory Management

- Uses dynamic memory allocation for variable storage and command arguments
- Implements proper memory cleanup to prevent leaks
- Resizes arrays as needed using `realloc()`

### I/O Redirection

- Uses `open()`, `dup2()`, and `close()` system calls to manage file descriptors for redirection
- Handles errors during file operations and terminates the command if any occur
- Supports multiple redirections in a single command line

### Process Management

- Creates child processes using `fork()`
- Executes commands with `execvp()` to automatically search PATH
- Parent process waits for child completion with `waitpid()`

## Building and Running

Compile the shell with:
```bash
gcc -o micro_shell main.c
```

Run the shell:
```bash
./micro_shell
```

## Usage Examples

```
Micro Shell Prompt > x=5
Micro Shell Prompt > echo $x
5
Micro Shell Prompt > folder=home
Micro Shell Prompt > ls /$folder > output.txt
Micro Shell Prompt > cat output.txt
# lists the /home directory contents
Micro Shell Prompt > echo "error" 2> error.txt
Micro Shell Prompt > cat error.txt
# displays error message
Micro Shell Prompt > x = 5
Invalid command
Micro Shell Prompt > exit
Good Bye
```

## Error Handling

- Validates variable assignments and reports syntax errors
- Checks for memory allocation failures
- Reports command execution errors
- Handles file system errors for built-in commands and I/O redirection

## License

AhmedWagdyMohy
