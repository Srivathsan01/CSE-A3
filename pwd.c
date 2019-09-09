#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<errno.h>

char* currentdir(char *s)
{
    getcwd(s,100);
    return s;
}