#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <signal.h>
#include "sys/wait.h"

#define USER ((const char *)getenv("USER"))
#define PWD ((const char *)getenv("PWD"))
#define OLDPWD ((const char *)getenv("OLDPWD"))
#define SIZE_PATH 256
#define SIZE_BUFFER 1024

char* getHostname(const char *filesystem);
int getPalabras(char *line);
void setWords(char *line, char **command_user, int total_words);
void checkCommand(char **command_user, int total_words);



//functions redirection
void output(int index, char **command_user);
void input(int index, char **command_user);

//fuction pipe
void executePipe(char **command_user, int total_words, int index);
bool ispipe(char **command_user, int total_words);

//fuction signals
void signalOff(void);
void signalOn(void);