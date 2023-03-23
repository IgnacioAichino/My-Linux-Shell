#include "structures.h"

int job_id= 0;


char* getHostname(const char *filesystem){

    FILE* fp= NULL;
    char buffer[100];

    fp= fopen(filesystem, "r");
    if(fp == NULL){
        fprintf(stderr, "No se pudo abrir el archivo");
        exit(1);
    }

    fgets(buffer, sizeof(buffer), fp);
    fclose(fp);

    //Delete \n of buffer
    if(strstr(buffer, "\n")){
        strcpy(buffer, strtok(buffer, "\n"));
    }

    return strdup(buffer);
}

int getPalabras(char *line){
    int words= 0;

    for(int i=0; i<strlen(line); i++){
        if(line[i] == ' '){
            if(line[i+1] != '\0' && line[i+1]!=' '){
                words++;
            }   
        }
    }

    return words;
}


void setWords(char *line, char **command_user, int total_words){
   
    if(total_words == 1){
        command_user[0]= strtok(line, "\n");
        return;
    }

    char *word="";
    command_user[0]= strtok(line, " ");

    for(int i=1; word!=NULL ; i++){
        word= strtok(NULL, " ");
        command_user[i]= word;
    
        //delete \n from last word
        if(i==total_words-1){
            strcpy(command_user[i], strtok(command_user[i], "\n"));
        }
    }

}

void checkCommand(char **command_user, int total_words){
    pid_t pid;
    pid= fork();

    //CHILD 
    if(pid==0){

        signalOn();
        char path_exec[100]= "/usr/bin/";
        bool flag_io= false;
        bool flag_pipe= ispipe(command_user, total_words);

        if(strcmp(command_user[total_words-1], "&") == 0){ 

            strcat(path_exec, command_user[0]);

            if(total_words == 2){                
                execl(path_exec, path_exec, NULL); 
            }
            else{            
                command_user[total_words-1]= NULL;
                execv(path_exec, command_user);
            }

        }
        else if((strcmp(command_user[0], "echo") == 0) && !flag_pipe){
            
            for(int i=1; i<total_words; i++){
                if(strcmp(">",command_user[i]) == 0){
                    flag_io= true;
                    output(i, command_user);
                    break;
                } 
                else if(strcmp("<",command_user[1]) == 0){
                    flag_io= true;
                    input(1, command_user);
                    break;
                }
            }

            if(!flag_io){
                if(total_words == 2 && strchr(command_user[1],'$') != NULL){
                    char *temp=  strtok(command_user[1],"$");
                    command_user[1]= getenv(temp);
                }   
                execvp(command_user[0], command_user);
            }
            
        }
        else if(strcmp(command_user[0], "clr") == 0){
            execl("/bin/bash", "sh", "-c", "clear", (char *)0); 
        }
        else if(flag_pipe){  
            for(int i=1; i<total_words; i++){
                if(strcmp(command_user[i], "|") == 0){
                    executePipe(command_user,total_words, i);
                }
            }
        }
        else{
            strcat(path_exec, command_user[0]);
            command_user[total_words-1]= NULL;
            execv(path_exec, command_user);
        }
        
        exit(0);
    }

    //FATHER
    else{

        if(strcmp(command_user[total_words-1], "&") == 0){
            job_id++;
            printf("Job ID [%d], Process ID %d\n", job_id, pid);
            //wait(NULL);
            return;
        }
        
        wait(0);
        if(strcmp(command_user[0],"cd") == 0){
            if(total_words > 2){
                fprintf(stderr, "myshell: cd: too many arguments\n");
            }
            else{       
                char const *new_path= command_user[1];
                if(strcmp(new_path, "-") == 0){
                    new_path= OLDPWD;
                }

                char old_path[SIZE_PATH];
                getcwd(old_path, SIZE_PATH);

                if( chdir(new_path) != 0) perror("ERROR: ");
                
                char cwd[SIZE_PATH];
                getcwd(cwd, SIZE_PATH);
                setenv("PWD", cwd, true);
                setenv("OLDPWD", old_path, true);
            }   
        }

        if(strcmp(command_user[0],"quit") == 0){
            exit(EXIT_SUCCESS);
        }
    }

}

