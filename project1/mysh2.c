#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


int main(){
    char cmd[100];  //Arxi tou command Line
    char* arguments2[100];        //var for the spliting
    while(strcmp(cmd, "exit")!=0){

    printf("$");                    //Shell Prompt

        if(!gets(cmd)){  
        break;                                
    }


    char *spliter;                  //splitter gia spasimo tis cmd me ta "";
    spliter = strtok(cmd," \t");
    int i=0;
    while(spliter!=NULL){
        arguments2[i]=spliter;      
        spliter = strtok(NULL," \t");
        i++;
    }
    arguments2[i]=NULL;                     //set last value to NULL for execvp

    int pid= fork();              

    if(pid==0 && (strcmp(cmd, "exit")!=0) ){               //Start of the Child
        execvp(arguments2[0],arguments2);
        fputs("No such command or directory\n", stderr);
        exit(1);
    }else{                    //Patir
        wait(NULL);
    }
}
return 0;
}

