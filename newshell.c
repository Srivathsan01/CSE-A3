#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/utsname.h>
#include <pwd.h>
#include <string.h>
#include <dirent.h>
#include <wait.h>
#include "headerfiles.h"


JBS JOBS[100];
int main()
{
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

    struct utsname udata;
    uname(&udata);

    char command[300];
    // char *curdir = (char *)calloc(100, sizeof(char));
    char curdir[100];
    char homedir[100];
    char username[30];
    gethostname(username, 30);
    realpath("/home/srivathsan/Desktop/CSE/A3", homedir);
    int HOMELEN = strlen(homedir);
    char dum[100];
    getcwd(dum, 100);
    if (strlen(dum) < HOMELEN)
        strcpy(curdir, dum);
    else if (strlen(dum) == HOMELEN)
    {
        int fr = 0;
        for (int l = 0; l < HOMELEN; l++)
        {
            if (dum[l] != homedir[l])
            {
                fr = 1;
                break;
            }
        }
        if (fr == 1)
            strcpy(curdir, dum);
        else
        {
            strcpy(curdir, homedir);
            curdir[HOMELEN] = '\0';
        }
    }
    else
        strcpy(curdir, dum);

    int numchildproc = 0;
    while (1)
    {
        char *temp = (char *)calloc(50, sizeof(char));
        char **commandsarray = (char **)calloc(10000, sizeof(char *));
        for (int i = 0; i < 20; i++)
        {
            commandsarray[i] = (char *)calloc(20, sizeof(char));
        }
        int numberofcommands = 0;

        struct passwd *p = getpwuid(getuid());
        if (strcmp(homedir, curdir) == 0)
        {
            strcpy(curdir, "~");
        }
        printf("<%s@%s:%s>", p->pw_name, username, curdir);

        fflush(stdin);
        scanf(" %[^\n]s", command);
        int amper = 0, inred = 0, outred = 0, pipe = 0;
        int REDIRECTFLAG = 0;
        display(command, commandsarray, &numberofcommands);

        for (int i = 0; i < numberofcommands; i++)
        {
            for (int halo = 0; halo < strlen(commandsarray[i]); halo++)
                if (commandsarray[i][halo] != ' ')
                {
                    commandsarray[i] = &commandsarray[i][halo];
                    break;
                }
            for (int rock = strlen(commandsarray[i]) - 1; rock > 0; rock--)
                if (commandsarray[i][rock] != ' ')
                {
                    commandsarray[i][rock + 1] = '\0';
                    break;
                }
            for (int g = 0; g < strlen(commandsarray[i]); g++)
            {
                if (commandsarray[i][g] == '&')             //0 means none occur
                    amper = 1;
                if (commandsarray[i][g] == '<')
                {
                    inred = 1;
                    REDIRECTFLAG = 1;                       // 1 means <
                }

                if (commandsarray[i][g] == '>')
                {
                    outred++;
                    REDIRECTFLAG = 2;                       // 2 means >
                    if(commandsarray[i][g+1] == '>')
                        {outred++;
                        g++;}
                }
                if (commandsarray[i][g] == '|')
                {
                    pipe = 1;
                    REDIRECTFLAG = 4;                     
                }
            }
            if(inred > 0 && outred > 0)
                REDIRECTFLAG = 3;                        // 3 means both occur

            addtohistory(commandsarray[i]);
            if(REDIRECTFLAG == 4)
            {
                pipes(commandsarray[i]);
            }
            if (strcmp(commandsarray[i], "pwd") == 0)
            {
                char *b = (char *)malloc(100 * sizeof(char));
                getcwd(b, 100);
                printf("%s\n", b);
                free(b);
            }
            else if (strncmp(commandsarray[i], "cd", 2) == 0 && commandsarray[i][2] == ' ')
            {
                char **arglist = (char **)malloc(4 * sizeof(char *));
                int d = 0;
                char *tk = strtok(commandsarray[i], " ");
                while (tk != NULL)
                {
                    arglist[d++] = tk;
                    tk = strtok(NULL, " ");
                }
                char *t = (char *)malloc(50 * sizeof(char));
                t = arglist[1];
                if (strcmp(t, "~") == 0)
                    changedir(homedir);
                else
                    changedir(t);
                getcwd(temp, 100);
                free(arglist);
                int throughhome = 1;

                if (strlen(temp) < HOMELEN)
                {
                    printf("%s\n", temp);
                    strcpy(curdir, temp);
                }

                if (strlen(temp) >= HOMELEN)
                {
                    // if(strcmp(t,"..") == 0 && strlen(t) == 2)
                    // {                                                                                   //cd ..
                    // int h;
                    // for(h=strlen(curdir); curdir[h] != '/'; h--);
                    // curdir[h]='\0';
                    // }
                    for (int g = 0; g < HOMELEN; g++)
                        if (temp[g] != homedir[g])
                        {
                            throughhome = 0;
                            break;
                        }
                    if (throughhome == 1)
                    {
                        int p, v;
                        strcpy(curdir, "~");
                        for (p = 1, v = HOMELEN; v < strlen(temp); p++, v++)
                        {
                            curdir[p] = temp[v];
                        }
                        curdir[p] = '\0';
                    }
                    else if (throughhome == 0)
                    {
                        printf("%s\n", temp);
                    }
                }
            }
            else if(strncmp(commandsarray[i] , "kjob" , 4) == 0)
            {
                kjob(commandsarray[i],JOBS,numchildproc);
            }
            else if(strncmp(commandsarray[i] , "jobs", 4) ==0)
            {
                jobs(JOBS,numchildproc);
            }
            else if (strncmp(commandsarray[i], "ls", 2) == 0 && REDIRECTFLAG != 4)
            {
                if (strlen(commandsarray[i]) == 2)
                {

                    list(REDIRECTFLAG, ".",outred);
                    continue;
                }
                int num = 0, dirpos = 0, outputpos = 0;
                int countl = 0, counta = 0, countd = 0;
                char **fldr = (char **)calloc(10, sizeof(char *));
                char *tok = (char *)malloc(80 * sizeof(char));
                tok = strtok(commandsarray[i], " ");
                while (tok != NULL)
                {
                    fldr[num] = tok;
                    num++;
                    tok = strtok(NULL, " ");
                }
                // free(tok);
                for (int x = 1; x < num; x++)
                {
                    if (strcmp(fldr[x], "-l") == 0)
                        countl++;
                    else if (strcmp(fldr[x], "-a") == 0)
                        counta++;
                    else if (strcmp(fldr[x], "-la") == 0 || strcmp(fldr[x], "-al") == 0)
                    {
                        countl++;
                        counta++;
                    }
                    else
                    {
                        if (strcmp(fldr[x], ">") != 0 && strcmp(fldr[x], "<") != 0 && strcmp(fldr[x], "|") != 0)
                            countd++;
                        if(countd == 1 && REDIRECTFLAG == 0)
                            dirpos = x;
                        if (countd == 1 && REDIRECTFLAG == 2)
                            {   
                                outputpos = num - 1;
                            }
                        if (countd == 2 && REDIRECTFLAG == 2)
                        {
                            outputpos = num - 1;
                            dirpos = num - 3;
                        }
                    }
                }
                if (countd > 0)
                {
                    char *mario = (char *)malloc(50 * sizeof(char));
                    getcwd(mario, 50);
                    if (dirpos != 0)
                    {
                    
                        if (strcmp(fldr[dirpos], "~") == 0)
                            chdir(homedir);

                        else
                        {
                            if (countl == 0 && counta == 0)
                                list(REDIRECTFLAG, fldr[dirpos],outred, fldr[outputpos]);
                            else if (countl == 0 && counta > 0)
                                listhidden(REDIRECTFLAG, fldr[dirpos],outred, fldr[outputpos]);
                            else if (countl > 0 && counta == 0)
                                longlist(0, REDIRECTFLAG, fldr[dirpos],outred, fldr[outputpos]);
                            else
                                longlist(1, REDIRECTFLAG, fldr[dirpos],outred ,fldr[outputpos]);
                        }
                
                        
                    }    
                
                    else if(dirpos == 0)
                    {
                        if (countl == 0 && counta == 0)
                            list(REDIRECTFLAG, ".",outred, fldr[outputpos]);
                        else if (countl == 0 && counta > 0)
                            listhidden(REDIRECTFLAG, ".",outred, fldr[outputpos]);
                        else if (countl > 0 && counta == 0)
                            longlist(0, REDIRECTFLAG, ".",outred, fldr[outputpos]);
                        else
                            longlist(1, REDIRECTFLAG, ".",outred, fldr[outputpos]);
                    }
                    chdir(mario);
                }
                else
                {
                        if (countl == 0 && counta == 0)
                            list(REDIRECTFLAG, ".",outred, fldr[outputpos]);
                        else if (countl == 0 && counta > 0)
                            listhidden(REDIRECTFLAG, ".", outred,fldr[outputpos]);
                        else if (countl > 0 && counta == 0)
                            longlist(0, REDIRECTFLAG, ".", outred,fldr[outputpos]);
                        else
                            longlist(1, REDIRECTFLAG, ".",outred, fldr[outputpos]);
                }
                
            }
            else if (strncmp(commandsarray[i], "pinfo", 5) == 0)
            {
                for (int u = 0; u < strlen(commandsarray[i]); u++)
                    if (commandsarray[i][u] != ' ')
                    {
                        commandsarray[i] = &commandsarray[i][u];
                        break;
                    }
                for (int m = strlen(commandsarray[i]) - 1; m > 0; m--)
                    if (commandsarray[i][m] != ' ')
                    {
                        commandsarray[i][m + 1] = '\0';
                        break;
                    }

                char **g = (char **)calloc(10, sizeof(char *));
                char *ben = (char *)malloc(80 * sizeof(char));
                int s = 0;
                ben = strtok(commandsarray[i], " ");
                while (ben != NULL)
                {
                    g[s] = ben;
                    s++;
                    ben = strtok(NULL, " ");
                }
                char opfname[30];
                if(REDIRECTFLAG == 2)
                    {
                        strcpy(opfname,g[s-1]);
                    }
                if (s > 1)
                {
                    char ww[10];
                    if(REDIRECTFLAG == 0)
                    {
                    strcpy(ww, g[1]);
                    }
                    if(REDIRECTFLAG == 2 && s==3)
                    {
                        int f = getpid();
                        char it[10] = "";
                        sprintf(it, "%d", f);
                        pinfo(it,REDIRECTFLAG,opfname,outred);
                    }
                    if(REDIRECTFLAG == 2 && s== 4)
                    {   strcpy(ww,g[1]);
                        pinfo(ww,REDIRECTFLAG,opfname,outred);
                    }
                    // long long ez = converttoint(ww);
                }
                else
                {
                    int f = getpid();
                    char it[10] = "";
                    sprintf(it, "%d", f);
                    pinfo(it,REDIRECTFLAG,opfname,outred);
                }
            }
            else if (strncmp(commandsarray[i], "echo", 4) == 0)
            {
                char opfname[30];
                char ww[100];
                int d,nm=0;
                for(d=5;d < strlen(commandsarray[i]); d++)
                    {   if(commandsarray[i][d] == '>')
                            {break;}
                        ww[d-5] = commandsarray[i][d];
                    }
                ww[d-5]='\0';
                if(REDIRECTFLAG == 2)
                {
                    for(int k=d+2; k < strlen(commandsarray[i]); k++)
                    {
                        opfname[nm++]=commandsarray[i][k];
                    }
                    opfname[nm]='\0';
                    if(outred == 2)
                    freopen(opfname,"w",stdout);
                    if(outred == 3)
                    freopen(opfname,"a+",stdout);
                }
               
                printf("\"%s\"\n", ww);
                if(REDIRECTFLAG == 2)
                {
                    freopen("/dev/tty","w",stdout);
                }
            }
            else if (strcmp(commandsarray[i], "history") == 0)
            {
                readhistory();
            }
            else if(strncmp(commandsarray[i], "setenv",6) == 0)
            {
                Setenv(commandsarray);
            }
            else if(strncmp(commandsarray[i], "unsetenv",8) == 0)
            {
                Unsetenv(commandsarray);
            }
            else if(strncmp(commandsarray[i],"quit",4) ==0)
            {
                overkill();
                exit(0);
            }
            else if(strncmp(commandsarray[i],"bg",2) ==0)
            {
                if(strlen(commandsarray[i]) == 2)
                {
                    printf("Usage: bg <jobnumber>\n");
                    continue;
                }
                bg(commandsarray[i],JOBS,numchildproc);
            }
            else if(strncmp(commandsarray[i],"fg",2) ==0)
            {
                if(strlen(commandsarray[i]) == 2)
                {
                    printf("Usage: fg <jobnumber>\n");
                    continue;
                }
                fg(commandsarray[i],JOBS,numchildproc);
            }
            else if(strcmp(commandsarray[i],"overkill") == 0)
            {
                overkill(JOBS,numchildproc);
            }
            else
            {
                char **F = (char **)calloc(10, sizeof(char *));
                char *spi = (char *)malloc(80 * sizeof(char));
                int b = 0;
                int ipos = 0, opos = 0;
                spi = strtok(commandsarray[i], " ");
                while (spi != NULL)
                {
                    F[b] = spi;
                    b++;
                    if (F[b] == '<')
                        ipos = b;
                    if (F[b] == '>')
                        opos = b;
                    spi = strtok(NULL, " ");
                }
                if (inred == 1 && outred == 0 && pipe == 0)             //Only input redirection
                {
                    int fr = open(F[b-1], O_RDONLY);        //ipfile
                    if(fr < 0)
                    {
                        perror(F[b-1]);
                        continue;
                    }            
                    int fdin = dup(STDIN_FILENO);
                    dup2(fr, 0);
                    close(fr);
                    int p=systemcommand(F,0,b,REDIRECTFLAG);
                    if( p > 1)
                        {addtojobs(JOBS,commandsarray[i],numchildproc,p);
                        numchildproc++;}
                    dup2(fdin, STDIN_FILENO);                    
                    close(fdin);
                }
                    
                else if (inred == 1 && outred == 1 && pipe == 0)
                {
                    int fdipfile = open(F[2], O_RDONLY);
                    if(fdipfile < 0)
                        {perror(fdipfile);
                         continue;
                        }
                    int fdopfile = open(F[4] , O_CREAT | O_WRONLY | O_TRUNC , 0766);
                    int fdstdin = dup(STDIN_FILENO);
                    dup2(fdipfile , 0);   //Making ipfile stdin
                    close(fdipfile);
                    int fdstdout = dup(STDOUT_FILENO);
                    dup2(fdopfile , 1);
                    close(fdopfile);
                    int p=systemcommand(F,0,b,REDIRECTFLAG);
                    if( p > 1){
                        addtojobs(JOBS,commandsarray[i],numchildproc,p);
                        numchildproc++;}
                    dup2(fdstdin , STDIN_FILENO);
                    dup2(fdstdout, STDOUT_FILENO);
                    close(fdstdin);
                    close(fdstdout);
                }
                else if (inred == 0 && outred == 0 && pipe == 0)
                {
                    
                    char syscommand[30];
                    strcpy(syscommand, F[0]);
                    if (b > 1)
                    {
                        if (amper == 1)
                        {
                            int r = systemcommand(F, 1, b,REDIRECTFLAG);
                            JOBS[numchildproc].procid = r; //Storing one more child pid
                            strcpy(JOBS[numchildproc].procname, F[0]);
                            JOBS[numchildproc].terminated = 0;
                            
                            numchildproc++;
                        }
                        else
                        {
                            int p=systemcommand(F, 0, b,REDIRECTFLAG);
                            if( p > 1)
                            {addtojobs(JOBS,commandsarray[i],numchildproc,p);
                             numchildproc++;}
                        }
                    }
                    else
                    {
                        int p =systemcommand(F, 0, b,REDIRECTFLAG);
                        if( p > 1)
                            {addtojobs(JOBS,commandsarray[i],numchildproc,p);
                             numchildproc++;}
                    
                    }
                }
            }
            /////////     CHECKING IF PROCESSES HAVE EXITED  ////////

            int status;
            int retid;
            for (int f = 0; f < numchildproc; f++)
            {
                retid = waitpid(JOBS[f].procid, &status, WNOHANG | WUNTRACED);
                if (retid > 0)
                { // Return if no child exits or child stops
                    if (WIFEXITED(status))
                    {
                        fprintf(stderr, "%s with pid %d exited normally\n", JOBS[f].procname, retid);
                        JOBS[f].terminated=1;
                    }
                    if (WIFSIGNALED(status))
                    {
                        fprintf(stderr, "%s with pid %d exited due to a signal\n", JOBS[f].procname, JOBS[f].procid);
                        JOBS[f].terminated=1;
                    }
                }
            }

            // printf("Current path %s\n",getcwd(temp,100));
        }
        // currentdir(curdir);
        free(commandsarray);
    }
    return 0;
}