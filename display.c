#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<errno.h>

void *display(char *str, char** commandsarray, int* numcommands)
{
    char *token = strtok(str,";");
    while(token != NULL)
    {
        // printf("%s\n",token);
        commandsarray[*numcommands] = token;
        *numcommands = *numcommands+1;
        token = strtok(NULL,";");
    }
}