// Implementing the cp command over linux using C and systemcalls



#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>



int main (int argc, char *argv[])
{
    if (argc < 3) // checking the arguments to be atleast two arguments cuz the argc[0] is the name of the program :)
    {
        printf("Usage: %s <source> <destination>\n", argv[0]);
        return 1;
    }
    int fd1 = open(argv[1], O_RDONLY); // opening the source file in read only mode
    if (fd1 == -1)
    {
        perror("open() error");
        return 1;
    }
    int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644); // opening the destination file in write only mode
    if (fd2 == -1)                                               // creating the file if it doesn't exist
    {
        perror("open() error");
        return 1;
    }
    // Check if the source and destination file descriptors are the same
    struct stat stat_src, stat_dst;
    if (fstat(fd1, &stat_src) == -1) {
        perror("fstat() error on source");
        close(fd1);
        close(fd2);
        return 1;
    }
    

     else if (stat_src.st_dev == stat_dst.st_dev && 
               stat_src.st_ino == stat_dst.st_ino) {
        fprintf(stderr, "Error: '%s' and '%s' are the same file\n", argv[1], argv[2]);
        close(fd1);
        close(fd2);
        return 1;
    }
    char buffer[1024];
    ssize_t bytes;
    while ((bytes = read(fd1, buffer, sizeof(buffer))) > 0) // reading the source file and writing the content to the destination file
    {
        if (write(fd2, buffer, bytes) != bytes)
        {
            perror("write() error");
            return 1;
        }
    }
    if (bytes == -1)
    {
        perror("read() error");
        return 1;
    }
    if (close(fd1) == -1) // closing the source file
    {
        perror("close() error");
        return 1;
    }
    if (close(fd2) == -1) // closing the destination file
    {
        perror("close() error");
        return 1;
    }
    return 0;
}


// Compile the code using the following command
// gcc main.c -o cp
// Run the code using the following command
// ./cp <source> <destination>
