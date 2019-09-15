#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<errno.h>
#include<dirent.h>
#include<time.h>
#include<pwd.h>
#include<grp.h>
#include<fcntl.h>

void list(int REFLAG,char *ipfname, char *outfname)
{
    struct dirent *den;    
    DIR *directory = opendir(ipfname);
    if(directory == NULL) {
        perror("Path");
        return;
    }
    while ((den = readdir(directory)) != NULL)
    { 
        if(den->d_name[0]!='.') 
        {
            if(REFLAG == 0)
            printf("%s\n", den->d_name); 
            else if(REFLAG == 2 || REFLAG == 3)
            {
                freopen(outfname,"a+",stdout);
                printf("%s\n", den->d_name); 
                freopen("/dev/tty","w",stdout);
            }
        }
    }
    closedir(directory);
}
// 
void listhidden(int REFLAG,char *ipfname,  char *outfname)
{
    struct dirent *den;
    DIR *D=opendir(ipfname);
    if (D == NULL)
    {
       perror(".");
       return;
    }
    if(REFLAG == 2)
    {
        freopen(outfname,"a+",stdout);
        while (( den = readdir(D)) != NULL)
        {   
          printf("%s\n",den->d_name);
        }
        freopen("/dev/tty","w",stdout);
    }

    else
    {
        while (( den = readdir(D)) != NULL)
        {   
          printf("%s\n",den->d_name);
        }
    }
    
    closedir(D);    
}
void longlist(int flag,int REFLAG,char *ipfname,  char *outfname)
{
    struct dirent *den;
    DIR *direc = opendir(ipfname);
    char *username =(char *)calloc(30,sizeof(char));
    gethostname(username,30);
    if(direc ==NULL)
    {
        printf("Error listing the files in the directory\n");
        return ;
    }
    if(REFLAG ==2  || REFLAG == 3)
    {  
        freopen(outfname,"a+",stdout);
    }
   
    while ((den = readdir(direc)) != NULL)
    {
        if(flag == 0 &&  den->d_name[0]=='.')
            continue;
        struct stat sri;
        stat(den->d_name,&sri);
        struct passwd *p = getpwuid(sri.st_uid);
        struct group *g =getgrgid(sri.st_gid);

        char filepermission[11];
        for(int c=0;c<10;c++)
            filepermission[c]='-';
        
        if(S_ISDIR(sri.st_mode))
            filepermission[0]='d';
        if(sri.st_mode & S_IRUSR)
            filepermission[1]='r';
        if(sri.st_mode & S_IWUSR)
            filepermission[2]='w';
        if(sri.st_mode & S_IXUSR)
            filepermission[3]='x';
        if(sri.st_mode & S_IRGRP)
            filepermission[4]='r';
        if(sri.st_mode & S_IWGRP)
            filepermission[5]='w';
        if(sri.st_mode & S_IXGRP)
            filepermission[6]='x';
        if(sri.st_mode & S_IROTH)
            filepermission[7]='r';
        if(sri.st_mode & S_IWOTH)
            filepermission[8]='w';
        if(sri.st_mode & S_IXOTH)
            filepermission[9]='x';  
        filepermission[10]='\0';
        char *tim = (char*)calloc(30,sizeof(char));
        strftime(tim,30,"%b %d %R",localtime(&sri.st_mtime));
        
        printf("%s %lld %s %s %5lld %s %s\r\n" ,filepermission,(long long)sri.st_nlink,p->pw_name,g->gr_name,(long long)sri.st_size,tim,den->d_name);
        
    }
    if(REFLAG == 2 || REFLAG == 3)
        {
            freopen("/dev/tty", "w" , stdout);
        }
}