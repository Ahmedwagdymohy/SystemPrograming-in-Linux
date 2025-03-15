
// Implementing the pwd command over linux using C and systemcalls

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>




int main(int argc, char *argv[])
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)  // getcwd() retruns a string containing the absolute path of the current working directory
    {
        printf("%s\n", cwd);
    }
    else
    {
        perror("getcwd() error");
        return 1;
    }
    return 0;
}


// Compile the code using the following command
// gcc main.c -o pwd
// Run the code using the following command
// ./pwd

