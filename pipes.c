// #include "headerfiles.h"

// void pipe(char *cmd)
// {
//     int pipepos[5],k=0;
//     for(int d = 0 ; d < strlen(cmd); d++ )
//     {
//         if(cmd[d] == '|')
//             pipepos[k++]=d;
//     }

//     char **args = (char **)malloc(70*sizeof(char));
//     char *tok = strtok(cmd," ");
//     int c = 0;
//     while(tok)
//     {
//         args[c] = tok;
//         tok = strtok(NULL, " ");
//         c++;
//     }
//     // for(int  h = 0)
// }