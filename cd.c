#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<errno.h>
#include<errno.h>

void changedir(char *pth)
{
    int r = chdir(pth);
    if(r<0)
    {
        perror(pth);
    }
}