#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/utsname.h>
#include <signal.h>
#include <sys/wait.h>    

int systemcommand(char **syscom,int flag, int len)
{
    
    char **argu = (char **)malloc(100*sizeof(char));
    for(int p=0;p < len; p++)
        argu[p] = (char*)malloc(40*sizeof(char));
        argu=syscom;
        argu[len]=NULL;
        // argu[] = NULL;
        if(flag==1)         // & was given
        {
            int status;
            int ind = fork();
            if(ind==0)
            {
                setpgid(0,0);       
                // printf("pid = %d\n",getpid());
                int d=execvp(argu[0],argu);            //Child execvp(command)
                if (d== -1)
                    printf("command not found: %s\n",syscom[0]);
            }
            else if(ind > 0)
            {
                waitpid(-1,&status, WNOHANG | WUNTRACED);
                return ind;                                             //return process id to shell
                // printf("Child pushed to background\n");
                ///USE KILL COMMAND kill(pid,0)
                //flag =1
            }
        }
        else
        {
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
                wait(NULL);
            }
        }
    
}