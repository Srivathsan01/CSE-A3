#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/utsname.h>
#include <pwd.h>
#include <string.h>
#include <dirent.h>
#include <wait.h>
int main()
{
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
        int childprocid[30];         //Array having all child pid's
        char childprocesses[30][20]; //Array containing all names of child processes
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
                if (commandsarray[i][g] == '&')
                    amper = 1;
                if (commandsarray[i][g] == '<')
                {
                    inred = 1;
                    REDIRECTFLAG = 1;
                }

                if (commandsarray[i][g] == '>')
                {
                    outred = 1;
                    REDIRECTFLAG = 2;
                }
                if (commandsarray[i][g] == "|")
                {
                    pipe = 1;
                    REDIRECTFLAG = 3;
                }
            }

            addtohistory(commandsarray[i]);

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

            else if (strncmp(commandsarray[i], "ls", 2) == 0)
            {
                if (strlen(commandsarray[i]) == 2)
                {

                    list(REDIRECTFLAG, ".");
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
                                list(REDIRECTFLAG, fldr[dirpos], fldr[outputpos]);
                            else if (countl == 0 && counta > 0)
                                listhidden(REDIRECTFLAG, fldr[dirpos], fldr[outputpos]);
                            else if (countl > 0 && counta == 0)
                                longlist(0, REDIRECTFLAG, fldr[dirpos], fldr[outputpos]);
                            else
                                longlist(1, REDIRECTFLAG, fldr[dirpos], fldr[outputpos]);
                        }
                
                        
                    }    
                
                    else if(dirpos == 0)
                    {
                        if (countl == 0 && counta == 0)
                            list(REDIRECTFLAG, ".", fldr[outputpos]);
                        else if (countl == 0 && counta > 0)
                            listhidden(REDIRECTFLAG, ".", fldr[outputpos]);
                        else if (countl > 0 && counta == 0)
                            longlist(0, REDIRECTFLAG, ".", fldr[outputpos]);
                        else
                            longlist(1, REDIRECTFLAG, ".", fldr[outputpos]);
                    }
                    chdir(mario);
                }
                else
                {
                        if (countl == 0 && counta == 0)
                            list(REDIRECTFLAG, ".", fldr[outputpos]);
                        else if (countl == 0 && counta > 0)
                            listhidden(REDIRECTFLAG, ".", fldr[outputpos]);
                        else if (countl > 0 && counta == 0)
                            longlist(0, REDIRECTFLAG, ".", fldr[outputpos]);
                        else
                            longlist(1, REDIRECTFLAG, ".", fldr[outputpos]);
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

                if (s > 1)
                {
                    char ww[10];
                    strcpy(ww, g[1]);
                    // long long ez = converttoint(ww);
                    pinfo(ww);
                }
                else
                {
                    int f = getpid();
                    char it[10] = "";
                    sprintf(it, "%d", f);
                    pinfo(it);
                }
            }
            else if (strncmp(commandsarray[i], "echo", 4) == 0)
            {
                char *u = &(commandsarray[i][5]);
                printf("\"%s\"\n", u);
            }
            else if (strcmp(commandsarray[i], "history") == 0)
            {
                readhistory();
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
                if (inred == 1 && outred == 0 && pipe == 0)
                {
                    if (strcmp(F[0], "sort") == 0)
                        systemcommand(F, 0, b);
                }
                else if (inred == 0 && outred == 1 && pipe == 0)
                {
                    // freopen(F[b], "a+",STDOUT);
                    // int fds;
                    // for(int l=0; l < opos; l++)
                    // {
                    // fds = open(F[0], O_RDONLY);
                    // dup2(fds,0);
                    // dup2(ofd,1);
                    // printf()
                    // }
                }
                else if (inred == 1 && outred == 1 && pipe == 0)
                {
                }
                if (inred == 0 && outred == 0 && pipe == 0)
                {
                    char syscommand[30];
                    strcpy(syscommand, F[0]);
                    if (b > 1)
                    {
                        if (amper == 1)
                        {
                            int r = systemcommand(F, 1, b);
                            childprocid[numchildproc] = r; //Storing one more child pid
                            strcpy(childprocesses[numchildproc], F[0]);
                            numchildproc++;
                        }
                        else
                        {
                            systemcommand(F, 0, b);
                        }
                    }
                    else
                    {
                        systemcommand(F, 0, b);
                    }
                }
            }
            int status;
            int retid;
            for (int f = 0; f < numchildproc; f++)
            {
                if ((retid = waitpid(childprocid[f], &status, WNOHANG | WUNTRACED)) > 0)
                { // Return if no child exits or child stops
                    if (WIFEXITED(status))
                    {
                        fprintf(stderr, "%s with pid %d exited normally\n", childprocesses[f], retid);
                    }
                    if (WIFSIGNALED(status))
                    {
                        fprintf(stderr, "%s with pid %d exited due to a signal", childprocesses[f], childprocid[f]);
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