#include "headerfiles.h"
void overkill(struct Job JOBS[100],int numchild)
{
    for(int t = 0 ; t < numchild ; t++)
    {
        if(JOBS[t].terminated != 1)
        {
            kill(JOBS[t].procid,SIGKILL);
        }
    }
}