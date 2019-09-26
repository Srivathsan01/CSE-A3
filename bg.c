#include "headerfiles.h"
bg(char *cmd,struct Job JOBS[100], int numchild)
{
    char *tok = strtok(cmd," ");
    tok = strtok(NULL," ");
    int jobnum  = atoi(tok),jobid;
    int present = 0;
    for(int c = 0 ; c < numchild ; c++ )
    {
        if(JOBS[c].jobnumber == jobnum  && JOBS[c].terminated == 0)
        {
            present = 1;
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
        kill(jobid, SIGCONT);
    }

}