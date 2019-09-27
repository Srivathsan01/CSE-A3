#include "headerfiles.h"

void pipes(char *cmd)
{
    int pipepos[5], numpipe = 0;
    int inred = 0, outred = 0;
    for (int d = 0; d < strlen(cmd); d++)
    {
        if (cmd[d] == '|')
            pipepos[numpipe++] = d;
        if (cmd[d] == '<')
            inred++;
        if (cmd[d] == '>')
            outred++;
    }
    int numberofcommands = numpipe + 1;
    char **microcom = (char **)malloc(70 * sizeof(char));
    char *tok = strtok(cmd, "|"); // Seperate all commands present between pipes
    int c = 0;
    while (tok)
    {
        microcom[c] = tok;
        for (int halo = 0; halo < strlen(microcom[c]); halo++)
            if (microcom[c][halo] != ' ')
            {
                microcom[c] = &microcom[c][halo];
                break;
            }
        for (int rock = strlen(microcom[c]) - 1; rock > 0; rock--)
            if (microcom[c][rock] != ' ')
            {
                microcom[c][rock + 1] = '\0';
                break;
            }

        tok = strtok(NULL, "|");
        c++;
    }
    //Declare  a pipe array for all commands
    int fdpipe[40];

    for (int j = 0; j < numpipe + 1; j++) //numpipe + 1 commands present
    {
        pipe(fdpipe + 2 * j);
    }
    for (int l = 0; l < numberofcommands; l++)
    {
            int count = 0;
            char *firstword = (char *)malloc(15 * sizeof(char));
            for (int h = 0; h < strlen(microcom[l]); h++)
            {
                if (microcom[l][h] == ' ')
                {
                    firstword[count] = '\0';
                    break;
                }
                firstword[count++] = microcom[l][h];
            }
            
            int argc = 0;
            char *argv[] = {NULL, NULL, NULL};
            //Splitting command into arguments
            char *token = strtok(microcom[l]," ");
            while(token)
            {
                argv[argc++] = token;
                token = strtok(NULL," ");
            }

            // printf("fitword: %s, Copied = %s\n", firstword, argv[0]);
        int ind = fork();
        if ( ind == 0)
        {
            if (l == 0)
            {
                //First command doesnt need stdin
                dup2(fdpipe[2 * l + 1], STDOUT_FILENO); //no need to dup2 stdin
            }

            else if (l == numberofcommands - 1)
            {
                //last command doent need stdout
                dup2(fdpipe[2 * l - 1], STDIN_FILENO);
            }
            else
            {
                dup2(fdpipe[2 * l - 1], STDIN_FILENO);
                dup2(fdpipe[2 * l + 1], STDOUT_FILENO);
            }
            for(int x = 0; x < numpipe + 1; x++){
                close(fdpipe[x]);
            }

            //seperate first word of command for argv[0]

            execvp(argv[0], argv);
            perror("execvp: ");

        }
        else
        {
            if(l == numberofcommands - 1) close(fdpipe[2*l + 1]);
            waitpid(ind, NULL, WUNTRACED);
        }
    }
}