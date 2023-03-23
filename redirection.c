#include "structures.h"


void output(int index, char **command_user){

    char buffer[SIZE_BUFFER]="";

    if(command_user[index+1] == NULL){
        fprintf(stderr, "myshell: echo: sintax error\n");
        return;
    }

    for(int i=1; i<index; i++){
        strcat(buffer, command_user[i]);
        strcat(buffer, " ");
    }


    FILE *fp;
    fp= fopen(command_user[index+1], "w");
    fputs(buffer,fp);
    fclose(fp);
}

void input(int index, char **command_user){
    
    if(command_user[index+2] != NULL){
        fprintf(stderr, "myshell: echo: sintax error\n");
        return;
    }

    char buffer[SIZE_BUFFER];

    FILE *fp;
    fp= fopen(command_user[index+1], "r");
    if(fp == NULL){
        fprintf(stderr, "No se pudo abrir el archivo\n");
        return;
    }

    fread(buffer, 1, SIZE_BUFFER, fp);
    fclose(fp);

    printf("%s\n", buffer);

}