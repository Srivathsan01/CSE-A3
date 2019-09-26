#include "headerfiles.h"

void handlectrlz(int sig_num)
{
    printf("CTRLZ SPOTTED\n");
    setpgid(0,0);
    pid_t pid = getpid();
    kill(pid, SIGSTOP);
}

int systemcommand(char **syscom,int flag, int len,int REDIR)
{
    
    char **argu = (char **)malloc(100*sizeof(char));
    for(int p=0;p < len; p++)
        argu[p] = (char*)malloc(40*sizeof(char));
        argu=syscom;
        argu[len]=NULL;
        if(REDIR == 1 || REDIR == 3)
        argu[1]=NULL;
        // argu[] = NULL;
        if(flag==1)         // & was given
        {
            int status;
            int ind = fork();
            if(ind==0)                              //child
            {
                setpgid(0,0);
                
                // printf("pid = %d\n",getpid());
                int d=execvp(argu[0],argu);            //Child execvp(command)
                if (d== -1)
                    printf("command not found: %s\n",syscom[0]);
            }
            else if(ind > 0)                                            // parent
            {
                waitpid(ind,&status, WNOHANG | WUNTRACED);
                return ind;                                             //return process id to shell
                // printf("Child pushed to background\n");
                ///USE KILL COMMAND kill(pid,0)
                //flag =1
            }
        }
        else
        {
            int status;
            int ind = fork();
            if(ind ==0)
            {
                // printf("I am child going into %s\n",syscom);
                int d=execvp(argu[0],argu);
                  if (d== -1)
                    printf("command not found: %s\n",syscom[0]);
                    //DIFF NOT WORKING
            }
            else
            {
                // printf("Parent waiting\n");
                waitpid(ind, &status, WUNTRACED);
                if(WIFEXITED(status) == 0)
                {
                    return ind;
                }
                return 0;
            }
        }
    
}