#include<stdio.h>
#include "headerfiles.h"

void Setenv(char **cmd)
{
    int length=0;
    char **R = (char **)malloc(100*sizeof(char*));
    int d=0;
    char *token = strtok(cmd[d]," ");
    while(token)
    {
        R[d++] = token;
        token = strtok(NULL," ");
    }
    length = d;
    char *variable = (char*)malloc(100*sizeof(char));
        variable = R[1];
    if(length == 2){
        setenv(variable,"",8);
    }
    else if(length == 3){
        char *overwrite = malloc(50*sizeof(char));
        overwrite = R[2];
        int re=setenv(variable, overwrite, 8);
        if(re == 0)
        printf("Successfully set %s as %s\n",variable , getenv(variable));
    }
}

void Unsetenv(char **cmd)
{
    char *token = strtok(cmd[0]," ");
    token = strtok(NULL," ");
    int re = unsetenv(token);
    // if(re == 0)
        // printf("Successfully unset\n");
}