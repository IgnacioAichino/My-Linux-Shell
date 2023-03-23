#include "structures.h"
#define CYAN "\033[036m"
#define WHITE  "\033[037m"

void batchfile(char *path, char **command_file, char *line, size_t line_size );

int main(int argc, char *argv[]){

    char *line= NULL;
    size_t line_size= 0;
    char **command_user=NULL;
    char dirActual[SIZE_PATH];


    
    
    //cantidad de comandos invalidos
    if(argc > 2){
        return -1;
    }

    if(argc > 1){
        //ejecuto linea de comandos por file
        batchfile(argv[1], command_user, line, line_size);
        return 0;
    }

    while(1){
        signalOff();
        
        getcwd(dirActual, SIZE_PATH);
        char *path=(char*)malloc(sizeof(dirActual));
        strcat(path, dirActual);

        //Check directorio /home/user
        if(strstr(path, getenv("HOME")) != NULL){  
            char temp[30]= "/home/";
            strcat(temp, getenv("USER"));
            path= path + strlen(temp);
            printf(CYAN);   
            printf("%s@%s:~%s$ ", USER, getHostname("/proc/sys/kernel/hostname"), path);
            printf(WHITE);
        }
        else{
            printf(CYAN);   
            printf("%s@%s:%s$ ", USER, getHostname("/proc/sys/kernel/hostname"), path);
            printf(WHITE);
        }


        line_size= getline(&line, &line_size , stdin);

        if(!strcmp(line, "\n")){
            continue;
        }
        else{
            //analizo arg in line;
            int total_words= getPalabras(line) + 1;
            command_user=(char**)malloc(sizeof(char*)*total_words);
           
            for(int i=0; i<total_words; i++){
                command_user[i]= (char*)malloc(sizeof(char)*30);
            }

            setWords(line, command_user, total_words);
            checkCommand(command_user, total_words);
            free(command_user);
        }
}
    
    return 0;
}


void batchfile(char *path, char **command_file, char *line, size_t line_size ){

    char dirActual[SIZE_PATH]; 
    FILE *fp= NULL;

    fp= fopen(path, "r");
    if(fp == NULL){
        fprintf(stderr, "No se pudo abrir el archivo");
        exit(1);
    }

    while( (line_size= getline(&line, &line_size , fp)) != -1){
 
        //Delete \n of buffer
        if(strstr(line, "\n")){
            strcpy(line, strtok(line, "\n"));
        }

        sleep(2);
        
        getcwd(dirActual, SIZE_PATH);
        char *path=(char*)malloc(sizeof(dirActual));
        strcat(path, dirActual);

        //Check directorio /home/user
        if(strstr(path, getenv("HOME")) != NULL){  
            char temp[30]= "/home/";
            strcat(temp, getenv("USER"));
            path= path + strlen(temp);
            printf(CYAN);   
            printf("%s@%s:~%s$ ", USER, getHostname("/proc/sys/kernel/hostname"), path);
            printf(WHITE);
            printf("%s\n", line);
        }
        else{
            printf(CYAN);   
            printf("%s@%s:%s$ ", USER, getHostname("/proc/sys/kernel/hostname"), path);
            printf(WHITE);
            printf("%s\n", line);
        }
 
      
        int total_words= getPalabras(line) + 1;
        command_file=(char**)malloc(sizeof(char*)*total_words);
        
        for(int i=0; i<total_words; i++){
            command_file[i]= (char*)malloc(sizeof(char)*30);
        }

        setWords(line, command_file, total_words);
        checkCommand(command_file, total_words);

        free(command_file);

        sleep(2);
    }

}