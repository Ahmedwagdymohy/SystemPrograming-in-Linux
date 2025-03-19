# Linux Shell Implementations

This repository contains a collection of shell implementations with varying levels of complexity and features, demonstrating system programming concepts in Linux.

## Repository Contents

### 1. Femto Shell
A minimal shell implementation that supports basic command execution:
- Built-in `echo` command
- `exit` command for termination
- Error handling for invalid commands

### 2. Pico Shell
A more advanced shell that builds upon Femto Shell:
- Built-in commands: `echo`, `pwd`, `cd`, and `exit`
- External command execution using fork/exec
- PATH resolution for commands
- Dynamic memory allocation for argument handling

### 3. Nano Shell
A feature-rich shell that extends Pico Shell with variable support:
- Variable assignment (`variable=value`)
- Variable substitution (`$variable`)
- Environment variable export with `export`
- All features from Pico Shell

### 4. Basic Linux Command Implementations
Individual implementations of common Linux commands:
- `pwd`: Print working directory
- `cp`: Copy files
- `mv`: Move files
- `echo`: Display text

## Technical Details

### System Calls Used
- `fork()`: Creates new processes
- `exec()`: Executes programs
- `wait()`: Waits for child processes
- `chdir()`: Changes directories
- `getcwd()`: Gets current working directory
- `open()`, `read()`, `write()`: File operations

### Memory Management
- Dynamic allocation for command arguments
- Variable storage with resizable arrays
- Proper memory cleanup to prevent leaks

### Process Management
- Parent-child process relationships
- Environment variable inheritance
- Process creation and termination

## Building and Running

Each shell implementation can be compiled separately:

```bash
# For Femto Shell
gcc -o femto_shell Femto_Shell/main.c

# For Pico Shell
gcc -o pico_shell Pico_Shell/main.c

# For Nano Shell
gcc -o nano_shell Nano_Shell/main.c
```

## Learning Progression

This repository demonstrates a progression in shell implementation complexity:

1. **Femto Shell**: Basic command parsing and built-in commands
2. **Pico Shell**: External command execution and directory navigation
3. **Nano Shell**: Variable management and environment manipulation

Each implementation builds upon the previous one, adding new features while maintaining the core functionality.

## Documentation

Each shell implementation has its own README file with detailed information:
- Feature descriptions
- Technical implementation details
- Usage examples
- Error handling

## License

AhmedWagdyMohy