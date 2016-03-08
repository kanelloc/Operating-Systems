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
int input[2], output[2];
int status;
int pId[100];
int i;

void read_1(int * input)
{
	dup2(input[0], STDIN_FILENO);
	close(input[1]);
}
void write_1(int * output)
{
	dup2(output[1], STDIN_FILENO);
	close(output[0]);
}

void fork_chain(int * input, int * output, int i)
{
	pId[i] = fork();

        if (pId[i] == 0)
        {
            if (input != NULL)
            {
                read_1(input);
            }

            if (output != NULL)
            {
                write_1(output);
            }

            execvp(arguments[i][0], arguments[i]);
            fputs("No such command or directory\n", stderr);
            exit(1);
        }
        else if (pId[i] < 0)
        {
            exit(1);
        }
}

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

        pipe(output);
        fork_chain(NULL, &output, 0);
        input[0] = output[0];
        input[1] = output[1];

        //for(i = 1; i < p - 1; i++)
        //{
        //	pipe(output);
        //	fork_chain(input, output, i);
        //	close(input[0]);
        //	close(input[1]);
        //	input[0] = output[0];
        //	input[1] = output[1];

        //}
        fork_chain(&input, NULL, p-1);
        close(input[0]);
        close(input[1]);
        waitpid(pId[p-1], &status, 0);
    }
    return 0;
}