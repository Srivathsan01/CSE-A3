                            ##########################################################################################################################
                                                                                ASSIGNMENT 2:
                            $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

    Requirement: TO build a shell having the given specifications

                                                                        ******Assumptions******:
1. I have assumed that for the background processes the & comes at the last after the arguments
2. cd - is not implemented.Rest all are implemented as cd - wasn't necessary.
3. I have assumed that the size of a sungle command which contains multiple minicommands seperated by ';' doesn't exceed 100 characters
4. The number of child processes does not exceed 30 .
5. If history.txt is not there , it is created and history gets stored there.
6. strtok() is used to parse strings


                                                                **************   MANUAL   *******************
1. The shell is named a.out
2. The shell can be run from anywhere.
3. The home directory is the place where shell resides indicated by ~. ~ is "/home/srivathsan/Desktop/CSE/A2"
4. If the shell starts anywhere except ~, the absolute path is shown in command line interface.Once we get into home directory.The functionality required gets implemented with "~/"
5. Modularity is implemented with each specification written in seperate c files. cd.c pwd.c ls.c echo.c pinfo.c systemcommands.c history.c display.c etc.
6. Make file is present in the same directory as the executable
7. The current directory is obtained through function getcwd()
8. 
    (1)The cd functionality is implemented through chdir() -change directory function.
    (2)We get all arguements and check for each arguement.I go to the directory with chdir(). IF the directory is inside ~ then appropriate value of curdir is fixed.Else, getcwd() is used
9.
    (1) For ls all arguments and flags are obtained. I count number of l ,a flags and directory.Then seperate processing is done for each case with  dirent structures and opendir() functions.
    (2) For directories opendir() closedir() and readdir() are used.
    (3) stat is used to print the number of hard links ,permissions , last modified date and time, userid, groupid etc.
10.
    (1) pinfo is implemented by going to the directory in which teh executable of the process with the given pid is present.We goto /proc/pid/stat. we get proess status and memory (3rd and 23rd values)
    (2) for executable path we use readlink() function to get to actual path of hardlinked exe 
11, 
    (1) history is implemented through file handling .A text file called history.txt is used.
    (2) Whenever history command is passed , we open the history.txt and read the last 10 lines of history.txt.( using fgets())
    (3) IF number of commands in it is already 20, we open a temp file, copy the last 19 commands and add one more new command.
    (4) The temp file is renamed with history.txt and old history.txt is removed.
12.
    (1) Foreground and background processes are done through forking and waiting
    (2) For foreground, we first fork the parent.The parent uses wait(NULL) until child terminates and control returns back.
    (3) Arguments can also be passded along with commands
    (4) For background, I store all the pid and processnames in an array.waitpid() is used to check for status of bg processes.
    (5) After adding processes i loop through all child processes to check which has ended and print the exit status using WIFEXITED(status)