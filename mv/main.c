#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int move_file(const char *src, const char *dest) {
    
    // Open the source file 
    int fd1 = open(src, O_RDONLY);
    if (fd1 == -1) {
        perror("open() error on source");
        return 1;
    }

    
    struct stat stat_src; // Get the source file's metadata to preserve permissions
    if (fstat(fd1, &stat_src) == -1) {
        perror("fstat() error on source");
        close(fd1);
        return 1;
    }

    
    int fd2 = open(dest, O_WRONLY | O_CREAT | O_TRUNC, stat_src.st_mode & 0777); // Open the destination file 
    if (fd2 == -1) { 
        perror("open() error on destination");
        close(fd1);
        return 1;
    }

    // Check if source and destination are the same file
    struct stat stat_dst;
    if (fstat(fd2, &stat_dst) != -1) {
        if (stat_src.st_dev == stat_dst.st_dev && stat_src.st_ino == stat_dst.st_ino) {
            fprintf(stderr, "Error: '%s' and '%s' are the same file\n", src, dest);
            close(fd1);
            close(fd2);
            return 1;
        }
    }


    // Copying
    char buffer[1024];
    ssize_t bytes;
    while ((bytes = read(fd1, buffer, sizeof(buffer))) > 0) {
        if (write(fd2, buffer, bytes) != bytes) {
            perror("write() error");
            close(fd1);
            close(fd2);
            return 1;
        }
    }

    if (bytes == -1) {
        perror("read() error");
        close(fd1);
        close(fd2);
        return 1;
    }

    // Close the files
    close(fd1);
    close(fd2);


    
    if (unlink(src) == -1) { // Remove the source file
        perror("unlink() error");
        return 1;
    }

    return 0; // Success
}






int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <source> <destination>\n", argv[0]);
        return 1;
    }

    const char *src = argv[1];
    const char *dest = argv[2];

    // Check if source exists
    struct stat stat_buf;
    if (stat(src, &stat_buf) == -1) {
        perror("stat() error on source");
        return 1;
    }


    if (S_ISDIR(stat_buf.st_mode)) {
        fprintf(stderr, "Error: '%s' is a directory (directory moves not supported in this implementation)\n", src);
        return 1;
    }

    if (stat(dest, &stat_buf) == 0 && S_ISDIR(stat_buf.st_mode)) {
        fprintf(stderr, "Error: '%s' is a directory\n", dest);
        return 1;
    }

    // Perform the move
    if (move_file(src, dest) != 0) {
        return 1;
    }

 
    return 0;
}