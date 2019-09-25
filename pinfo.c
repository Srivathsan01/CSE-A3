#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<math.h>

void pinfo(char* id,int REFLAG, char opfname[30])
{
    char procpath[30] = "/proc/",exepath[20]="", statpath[] = "/stat";
    strcat(procpath,id);
    strcpy(exepath,procpath);
    strcat(procpath,statpath);

    FILE *fl = fopen(procpath, "r");
    if(fl == NULL)
    {
        perror(procpath);
        return ;
    }
    char tempstr[30],status[30],virmem[30];
    int ct=1;
    while(fscanf(fl,"%s",tempstr))
    {
        if(ct == 3)
        {
            strcpy(status,tempstr);
        }
        if(ct == 23)
        {
            strcpy(virmem,tempstr);
            break;
        }
        ct++;
    }
    char buffer[40];
    strcat(exepath,"/exe");
    readlink(exepath,buffer,40);

    if(REFLAG == 2)
    {
        freopen(opfname,"a+",stdout);
    }
    printf("pid -- %s\n",id);
    printf("Process Status -- %s\n",status);
    printf("memory -- %s\n",virmem);
    printf("executable path -- %s\n",buffer);
    if(REFLAG == 2)
    {
        freopen("/dev/tty","w",stdout);
    }
    // fscanf(fl, "%d" , &pid)
    fclose(fl);
}
