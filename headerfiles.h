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

struct Job
{
    int procid;
    char procname[30];
    int terminated;
};
typedef struct Job JBS;