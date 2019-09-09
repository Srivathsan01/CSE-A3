#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/utsname.h>

void addtohistory(char *newcommand)
{
    char *com=(char *)malloc(100*sizeof(char));
    com = newcommand;
    char str[100];
    int numcommands = 0;
    FILE* fptr;
    FILE* temp;
    if((fptr=fopen("/home/srivathsan/Desktop/CSE/A2/history.txt", "a+")) == NULL)
    {                                                                                           //Read + Write the history.txt
        perror("history.txt");
    }
    if((temp = fopen("/home/srivathsan/Desktop/CSE/A2/temp.txt", "w")) == NULL)
    {                                                                                           //Temp file.Take last 19 entries to temp.txt add the new command and rename it to history
        perror("temp.txt");
    }
    while( fgets(str,100, fptr) != NULL)
    {
        numcommands++;                                                                                  //Checks number of commands
    }    
    fseek(fptr,0,SEEK_SET);
    if(numcommands == 20)
    {
        char dump[100]="";
        fgets(dump,100, fptr);                                                                  //first line is garbage
        while( fgets(str,100, fptr) != NULL)
        {
            fputs(str,temp);
        }
        
        fputs(newcommand,temp);             //Last line the new command is added
        
        if(remove("history.txt") == 0)
        {
            // printf("Old file removed\n");
        }
        if(rename("temp.txt","history.txt") == 0)
        {
            // printf("Rename successful\n");
        }
    }
    else if(numcommands < 20)
    {
        char b[50];
        strcpy(b,newcommand);
        fputs(b,fptr);
        fputc('\n',fptr);
    }
    
    fclose(fptr);
    fclose(temp);
}
void readhistory()
{
    FILE* fptr;
    fptr=fopen("/home/srivathsan/Desktop/CSE/A2/history.txt", "r");
    if(fptr == NULL)
    {
        perror("fptr");
        return;
    }
    int numcommands = 0;
    char str[100];
    int count=1;
    while( fgets(str,100, fptr) != NULL)
    {
        numcommands++;                                                                                  //Checks number of commands
    }    
    fseek(fptr,0,SEEK_SET);
    if(numcommands <=10)
    {
        while( fgets(str,100, fptr) != NULL && count <=10)
        {
         count++;
         printf("%s\n",str);
        }
    }
    else
    {
        char dummy[100];
        for(int j=0;j<(numcommands-10);j++)
            fgets(dummy,100,fptr);
        while(fgets(str,100,fptr) != NULL)
            printf("%s\n",str);
    }
        
    
}