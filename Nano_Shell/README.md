# Nano Shell

A feature-rich shell implementation in C that supports built-in commands, external program execution, and variable management.

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

- Uses dynamic memory allocation for variable storage
- Implements proper memory cleanup to prevent leaks
- Resizes arrays as needed using `realloc()`

### Variable Handling

1. **Variable Assignment**:
   - Parses input to identify variable assignments (`name=value`)
   - Validates variable names (must start with letter/underscore, contain only alphanumeric/underscore)
   - Stores variables in the `VarTable` structure

2. **Variable Substitution**:
   - Detects `$variable` syntax in command arguments
   - Replaces variables with their values before command execution
   - Returns empty string for undefined variables

3. **Environment Variables**:
   - `export` command adds shell variables to process environment
   - Child processes inherit all exported environment variables

## Building and Running

Compile the shell with:
```bash
gcc -o nano_shell main.c
```

Run the shell:
```bash
./nano_shell
```

## Usage Examples

```
Nano Shell Prompt > x=5
Nano Shell Prompt > echo $x
5
Nano Shell Prompt > folder=home
Nano Shell Prompt > ls /$folder
# lists the /home directory
Nano Shell Prompt > export x
Nano Shell Prompt > printenv | grep x
x=5
Nano Shell Prompt > x = 5
Invalid command
Nano Shell Prompt > exit
Good Bye
```

## Error Handling

- Validates variable assignments and reports syntax errors
- Checks for memory allocation failures
- Reports command execution errors
- Handles file system errors for built-in commands 