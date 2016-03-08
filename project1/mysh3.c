#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <unistd.h>

char cmd[100];
char *arguments[100][100];
char *arguments2[100];
char *spliter1;
char *spliter2;
int i, output[2];
int status;


int main()
{
    while (strcmp(cmd, "exit") != 0)
    {
        printf("$");                    //Shell Prompt

        if(!gets(cmd)){  
        break;
        }


        int p = 0;
        spliter1 = strtok(cmd,"|");
        while (spliter1 != NULL)
        {
            arguments2[p] = spliter1;
            spliter1 = strtok(NULL, "|");
            p++;
        }


        for(i = 0; i < p; i++)
        {
            int k = 0;
            spliter2 = strtok(arguments2[i], " ");
            while (spliter2 != NULL)
            {
                arguments[i][k] = spliter2;
                spliter2 = strtok(NULL, " ");
                k++;
            }
            arguments[i][k] = NULL;
        }
        //-----------------------------------------------------------------------------//
        pipe(output);
        int pid = fork();
        if (pid == 0){
            if(p > 1){
                dup2(output[0],STDIN_FILENO);   
                close(output[1]);        
                execvp(arguments[1][0], arguments[1]);
                exit(1);


            }
            else{
                execvp(arguments[0][0], arguments[0]);
                exit(1);
            }
        }
        
        else{
            int pid2 = fork();
            if(pid2 == 0){
                dup2(output[1],STDOUT_FILENO);
                close(output[0]);
                execvp(arguments[0][0], arguments[0]);
                exit(1);
            }
            close(output[1]);
            close(output[0]);
            int chid = waitpid(pid, &status, 0);
        }
    }
    return 0;
}
