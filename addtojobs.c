#include "headerfiles.h"

void addtojobs(struct Job JOBS[100],char *cmd,int numchild,int pid)
{
    JOBS[numchild].procid = pid;
    strcpy(JOBS[numchild].procname,cmd);
    JOBS[numchild].terminated = 0;
}