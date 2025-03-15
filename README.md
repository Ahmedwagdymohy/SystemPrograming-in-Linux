# System programming under Linux
## 1. Implementation of the basic commands of the Linux shell
## 1. pwd 

This is a simple implementation of the `pwd` (Print Working Directory) command in Linux using C and system calls. prints the absolute path of the current working directory.


## Code Overview

- **`getcwd()`**: The core function used to retrieve the current working directory. It stores the absolute path in the provided buffer.
- **Error Handling**: If `getcwd()` fails, the program prints an error message using `perror()` and exits with a non-zero status.


## Output
The program will print the absolute path of the current working directory to the terminal.

## Error Handling
If `getcwd()` fails (e.g., due to insufficient permissions), the program will print an error message and exit with a status code of `1`.





## 2. cp
implementation of the cp (Copy) command in Linux using C and system calls. The program copies the contents of a source file to a destination file.


This is a simple implementation of the `cp` (Copy) command in Linux using C and system calls. The program copies the contents of a source file to a destination file.


## Code Overview
- **`open()`**: Opens the source file in read-only mode and the destination file in write-only mode (creating it if it doesn't exist).
- **`read()` and `write()`**: Reads data from the source file and writes it to the destination file.
- **Error Handling**: Checks for errors during file operations and prints appropriate error messages.
- **File Comparison**: Ensures the source and destination files are not the same to avoid data loss.


## Output
The program will copy the contents of the source file to the destination file. If the destination file does not exist, it will be created.

## Error Handling
- If the source or destination file cannot be opened, an error message is printed.
- If the source and destination files are the same, an error message is printed to prevent data loss.
- Errors during reading or writing operations are also handled with appropriate error messages




## 3. mv
simple implementation of the `mv` (Move) command in Linux using C and system calls. The program moves a file from a source location to a destination location by copying the file and then deleting the original.


## Code Overview

- **`open()`**: Opens the source file in read-only mode and the destination file in write-only mode (creating it if it doesn't exist).
- **`read()` and `write()`**: Reads data from the source file and writes it to the destination file.
- **`unlink()`**: Deletes the source file after a successful copy.
- **Error Handling**: Checks for errors during file operations and prints appropriate error messages.
- **File Comparison**: Ensures the source and destination files are not the same to avoid data loss.


## Output
The program will move the source file to the destination location. If the destination file already exists, it will be overwritten.

## Error Handling
- If the source or destination file cannot be opened, an error message is printed.
- If the source and destination files are the same, an error message is printed to prevent data loss.
- Errors during reading, writing, or deleting operations are also handled with appropriate error messages.


## 4. echo


This is a simple implementation of the `echo` command in Linux using C. The program prints the provided arguments to the standard output.

## Code Overview

- **Argument Handling**: The program checks if at least one argument is provided (excluding the program name).
- **Loop Through Arguments**: Iterates through the provided arguments and prints them to the console.
- **Error Handling**: If no arguments are provided, the program prints a usage message and exits with an error code.





## Output
The program will print the provided string(s) to the terminal, followed by a newline.

## Error Handling
- If no arguments are provided, the program prints a usage message and exits with a status code of `1`.
