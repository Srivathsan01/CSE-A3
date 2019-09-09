#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<errno.h>
#include<sys/utsname.h>
#include<pwd.h>
#include<string.h>

int main()
{   
    struct utsname udata;
    uname(&udata);

    char *command = (char *)calloc(30,sizeof(char));
    char *temp = (char *)calloc(100,sizeof(char));
    char *curdir = (char*)calloc(100,sizeof(char));
    char *homedir = (char *)calloc(100,sizeof(char));
    char *username =(char *)calloc(30,sizeof(char));
    gethostname(username,30);
    getcwd(homedir,100);
    curdir[strlen(homedir)]='\0';
    strcpy(curdir,homedir);
    while(1)
    {
    char **commandsarray = (char **)calloc(100,sizeof(char*));
    for(int i=0; i<20 ;i++)
    {
        commandsarray[i] = (char*)calloc(20,sizeof(char));
    }
    int numberofcommands = 0;

    struct passwd *p = getpwuid(getuid());
    if (strcmp(homedir,curdir)==0)
        {strcpy(curdir,"~");}
    printf("<%s@%s:%s>", p->pw_name, username, curdir);
    
    fflush(stdin);
    scanf(" %[^\n]s",command);
    display(command,commandsarray,&numberofcommands);
    for(int i=0;i<numberofcommands; i++)
    {
        if(strcmp(commandsarray[i], "pwd") ==0 )
         {
            printf("%s\n",getcwd(temp,100));
         }
        else if (strncmp(commandsarray[i],"cd",2)==0)
        {
            // printf("cd being executed\n");
            if(strlen(commandsarray[i]) == 2 || (strlen(commandsarray[i])== 3 && commandsarray[i][2]==' '))
                strcpy(curdir,"~");
            else{            
            char *t=malloc(50*sizeof(char));
            t=&commandsarray[i][3];
            changedir(t);
            if(strcmp(t,"..") == 0 && strlen(t) == 2)
            {                                                                                   //cd ..
                int h;
                for(h=strlen(curdir); curdir[h] != '/'; h--);
                curdir[h]='\0';
            }
            if(t[0] == '.' && strlen(t) > 2)
            {
                if(t[1]=='.')                                                                    //cd ../dasf/asfasf
                {
                    for(int j=strlen(curdir),k=2; j < strlen(curdir)+ strlen(t)-2; j++)
                    {
                        curdir[j]=t[k++];
                    }
                }
                else                                                                                //cd ./tempo/azure
                {
                    for(int j=strlen(curdir),k=1; j < strlen(curdir)+ strlen(t)-2; j++)
                    {
                        curdir[j]=t[k++];
                    }
                }
            }
            else                                                                                    //cd movies
            {   curdir[strlen(curdir)] = '/';
                //  for(int j=strlen(curdir)+1,k=0; j < strlen(curdir)+ strlen(t) + 1; j++)
                //     {
                //         curdir[j]=t[k++];
                //     }
                strcat(curdir,t);
            }
            }
            printf("Current path %s\n",getcwd(temp,100));
        }
       /*else if (strcmp(commandsarray[i],"ls")==0))
        {
            
        }
        */
        else if (strncmp(commandsarray[i],"echo",4)==0)
        {
            char *u=&(commandsarray[i][5])  ;
            if(commandsarray[i][5] == '\"' && commandsarray[i][strlen(commandsarray[i]) - 1] == '\"')
            {  
                printf("%s\n",u);
            }
            else
            printf("\"%s\"\n",u);
        }
       /*else if (strcmp(commandsarray[i],"cd")==0))
        {
            
        }
         */
        else
        {
            printf("Command not Found: %s\n",commandsarray[i]);
        }
        

    }
    // currentdir(curdir);
    free(commandsarray);
    }
    return 0;
}