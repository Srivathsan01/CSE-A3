#include "headerfiles.h"

void fg(char *cmd,struct Job JOBS[100], int numchild)
{
    char *tok = strtok(cmd," ");
    tok = strtok(NULL," ");
    int jobnum  = atoi(tok),jobid;
    int present = 0,position;
    pid_t shellpid = getpid();
    for(int c = 0 ; c < numchild ; c++ )
    {
        if(JOBS[c].jobnumber == jobnum  && JOBS[c].terminated == 0)
        {
            present = 1;
            position = c;
            jobid = JOBS[c].procid;
            break;
        }
    }
    if(present == 0)
    {
        printf("No Such Job Exists\n");
        return;
    }
    if(present == 1)
    {
        signal(SIGTTOU, SIG_IGN);
        tcsetpgrp(0, getpgid(jobid));
        kill(jobid, SIGCONT);
        waitpid(jobid , NULL , WUNTRACED);
        JOBS[position].terminated = 1;
        tcsetpgrp(0, getpgid(shellpid));
        if(strcmp(JOBS[position].procname,"vim &") == 0)
        printf("vim with pid %d exited due to signal\n");
        signal(SIGTTOU, SIG_DFL);
    }
}