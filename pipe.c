#include "structures.h"
#include <unistd.h>


bool ispipe(char **command_user, int total_words){
    for(int i=1; i<total_words; i++){
        if(strcmp(command_user[i], "|") == 0){
            return true;
        }
    }

    return false;
}

void executePipe(char **command_user, int total_words, int index){

    char path_exec[100]= "/usr/bin/";
     
    int fd[2];
    pipe(fd);

    int pid= fork();

    //CHILD process
    if(pid==0){
        strcat(path_exec, command_user[0]);

        char **command=(char**)malloc(sizeof(char*)*total_words);
        for(int i=0; i<index; i++){
            command[i]= command_user[i];
        }

        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
    
        execv(path_exec, command);
        exit(1);
    }
    else{ //Father process

        
        strcat(path_exec, command_user[index+1]);

        char **command=(char**)malloc(sizeof(char*)*total_words);  
        int j=0;
        for(int i=index+1; i<total_words; i++){
            command[j]= command_user[i];
            j++;
        }
        
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);

        execv(path_exec, command);
        exit(1);
    }
}