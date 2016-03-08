#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


int main(){
    char cmd[100];  //Arxi tou command Line
    while(strcmp(cmd, "exit")!=0){

    printf("$");                    //Shell Prompt

        if(!gets(cmd)){  
        break;                                
    }

    int pid= fork();              

    if(pid==0 && (strcmp(cmd, "exit")!=0) ){               //Start of the Child
        execlp(cmd,cmd,NULL);
        fputs("No such command or directory\n", stderr);
        exit(1);
    }else{                    //Patir
        wait(NULL);
    }
}
return 0;
}

