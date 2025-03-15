// implementing the echo command over linux using C and systemcalls


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>



int main(int argc, char *argv[])
{
    if (argc < 2) // checking the arguments to be atleast one argument cuz the argc[0] is the name of the program :)
    {
        printf("Usage: %s <string>\n", argv[0]);
        return 1;
    }
    for (int i = 1; i < argc; i++)
    {
        printf("%s ", argv[i]);
    }
    printf("\n");
    return 0;
}


// Compile the code using the following command
// gcc main.c -o echo
// Run the code using the following command
// ./echo <string>
