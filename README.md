### Laboratorio N5
# MyShell Advanced
### SOI - FCEFyN - UNC - 2022


## Compilacion
*Para la compilacion de este proyecto debera utilizar GCC*

Dentro de este repositorio se encuentra el archivo *Makefile* que le permitira compilar.

Comandos necesarios:

*Compilacion*
- make (Realiza la compilacion)

Este comando genera un ejecutable llamado **myshell**

#### Ejecucion

- ./myshell (Ejecuta el programa)

Al comenzar programa le devolvera  un prompt para ejecucion de comandos internos de linux mediante stdin.

_username@hostname:~$_

**myshell** soporta diferentes comandos alguno de ellos por ejemplo son:
- **cd:** Cambia el directorio actual. Ej: _cd /home/_ actualizando la variable de        entorno PWD al nuevo directorio.
    Podra tambien ejecutar el comando _cd -_ para regresar al ultimo directorio de trabajo a partir de la varible de entorno OLDPWD actual.
- **clr:** Este comando limpia la pantalla.
- **echo:** Permite mostrar un comentario/string en la pantalla seguido de una linea      nueva. Ej _echo este es un comentario_  
    Tambien puede obtener el valor de variables de entornos del sistema mediante este comando. Ej _echo $HOME_
- **quit:** Cierra el programa _myshell_
- **ls**: ver directorio actual
- **top**: procesos en ejecucion en tiempo real

Cuenta con la posibilidad de ejecutar aplicaciones en primer plano. 
Por ejemplo:

_username@hostname:~$_  firefox

#### Background execution

Un ampersand & al final de la línea de comando indica que la shell debe retornar al prompt inmediatamente luego de lanzar al programa a ejecutarse en background.
Cuando se realiza este comando se imprime el _JobID y ID de proceso_.
Ej: _firefox &_

#### Signal Handling
En caso de que ingrese  CTRL-C, CTRL-Z o CTRL-/, las señales resultantes (SIGINT, SIGTSTP, SIGQUIT respectivamente) son enviadas al trabajo en primer plano ejecutando sino no realiza nada. 

Para hacer uso de esta funcionalidad se utiliza la libreria _#include <signal.h>_.
Cuando se realiza la ejecucion del proceso hijo que lleva a cabo las tareas de ejecucion de los comandos ingresados por stdin, son activadas estas señales para que puedan actuar en caso de ser ingresadas. 
En cambio, si la ejecucion se realiza en el proceso padre estas señales no estan habilitadas.
Las siguientes funciones son las que activan y desactivan las señales mencionadas.
Estas mismas son invocadas en los respectivos procesos dichos.
```C
void signalOff(void){
    signal(SIGINT,SIG_IGN);
    signal(SIGTSTP,SIG_IGN);
    signal(SIGQUIT,SIG_IGN);
}

void signalOn(void){
    signal(SIGINT,SIG_DFL);
    signal(SIGTSTP,SIG_DFL);
    signal(SIGQUIT,SIG_DFL);   
}
```

#### Batchfile

El programa **myshell** es capaz de tomar comandos a partir de un archivo invocado como argumento. Ej: _./myshell batchile_ 
De esta manera el programa lee los coamandos indicados en las lineas del archivo y podra visualizar la ejecucion de los mismo. Cuando se alcance el final del archivo el programa myshell se cerrara.

**NOTA:** Como se puede observar _myshell_ puede ejecutarse mediante un argumento que es un batchifle o sin argumentos para q ingrese comandos mediante stdin.


#### Pipe

**myshell** provee la funcionalidad de un pipe a través del operador **| (pipe)**. El mismo conecta la salida estándar del proceso (stdout) lazando por el comando de la izquierda del pipe con la entrada estándar del proceso (stdin) que se genera con el comando a la derecha del pipe.
Esto es un mecanismo de IPC unidireccional implementado mediante la creacion de un procesos hijo a partir de un proceso padre el cual tiene un pipe asociado a dos filedescriptor que nos permite la escritura y lectura en los extremos del pipe para lograr esta funconalidad.

EJ:
_username@hostname:~$_  ls -l | wc

```C
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
```

Los pipes en el filesystem se encuentran en:  
 - /proc/sys/fs/pipe-max-pages  
 - /proc/sys/fs/pipe-max-size  
 - /proc/sys/fs/pipe-user-pages-hard  
 - /proc/sys/fs/pipe-user-pages-soft

#### I/O redirection 

**myshell** soportar la redirección de entrada/salida en stdin y/o stdout a travez del comando echo.

_program arg1 ar2 < inputfile > outputfile_

stdin es reemplazado por inputfile y stdout por outputfile

EJ:

_username@hostname:~$_  echo hola mundo > file

_username@hostname:~$_  echo < file

### Objetivo y Conclusiones 

El principal objetivo de este proyecto en Lenguaje C, fue desarrolar una shell avanzada que interprete un conjunto de comandos o sean indicados estos comandos por un archivo. Para lograr estos objetivos se necesito comprender el funcionamiento de los procesos en un Sistema Operativo, como asi tambien como son administrados estos procesos en tiempo de ejecucion por el Sistema Operativo. Estas tareas se logran realizando llamadas al sistema para poder interectuar desde el espacio de usuario con el kernel. Gracias a estas llamadas de sistema se puede obtener privilegios a nivel kernel y poder realizar tareas privilegiadas, permitiendo la ejecucion de nuestros comandos, como tambien ejecutar en segundo plano una aplicacion y continuar con la ejecucion de nuestra shell.
Este repositorio cuenta con los files .c y .h que han sido desarrollados para lograr las funcionalidades ya mencionadas. 
Cuenta con Makefile para compilar y limpiar el directorio de los archivos generados con el comando _make clean_ 
 
