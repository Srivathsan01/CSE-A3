#include "headerfiles.h"
void kjob(char *cmd, struct Job JOBS[100],int numchild)
{
    char **ar = (char **)malloc(50*sizeof(char));
    char *tok = strtok(cmd," ");
    int ct=0;
    while(tok)
    {
        ar[ct++] = tok;
        tok = strtok(NULL," ");
    }
    int jobn,signumber;
    jobn = atoi(ar[1]);
    signumber = atoi(ar[2]);
    for(int w=0; w < numchild ; w++)
    {
        if(JOBS[w].jobnumber == jobn && JOBS[w].terminated == 0)
        {
            kill(JOBS[w].procid,signumber);
            break;
        }
    }
}