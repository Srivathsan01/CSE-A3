#include "headerfiles.h"

void jobs( struct Job JOBS[100],int numchild)
{
    int jnumber = 0;
    int terminated = 0;
    char jobstatus[100];
    memset(jobstatus, 0, 100);
    for(int f=0; f<numchild; f++)
    {
        if(JOBS[f].terminated == 1)
        {
            terminated++;
        }
        if(JOBS[f].terminated != 1)
        {
            JOBS[f].jobnumber = f+1-terminated;
            char procpath[30] = "/proc/",exepath[20]="", statpath[] = "/stat";
            char jobid[10];
            sprintf(jobid,"%d",JOBS[f].procid);
            strcat(procpath,jobid);
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
                    break;
                }
                ct++;
            }
            if(strcmp(status,"T") == 0)
            strcpy(jobstatus,"Stopped");
            
            if(strcmp(status,"D") == 0)
            strcpy(jobstatus,"Waiting");
            
            if(strcmp(status,"Z") == 0)
            strcpy(jobstatus,"Zombie");
            
            if(strcmp(status,"X") == 0)
            strcpy(jobstatus,"Dead");

            if(strcmp(status,"K") == 0)
            strcpy(jobstatus,"Wakekill");

            if(strcmp(status,"S") == 0)
            strcpy(jobstatus,"Sleeping");
            
            if(strcmp(status,"P") == 0)
            strcpy(jobstatus,"Parked");

            if(strcmp(status,"R") == 0)
            strcpy(jobstatus,"Running");
            printf("[%d] %s %s [%d]\n",JOBS[f].jobnumber,jobstatus,JOBS[f].procname, JOBS[f].procid);
        }
    }
}