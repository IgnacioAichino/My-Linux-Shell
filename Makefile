CFLAGS= -Wall -Werror -pedantic

myshell: main.o functions_struc.o redirection.o pipe.o signals.o
		gcc $(CFLAGS) -o myshell main.o functions_struc.o redirection.o pipe.o signals.o

main.o: main.c
		gcc $(CFLAGS) -c main.c
	
functions_struc.o: functions_struc.c 
		gcc $(CFLAGS) -c functions_struc.c

signals.o: signals.c		
		gcc $(CFLAGS) -c signals.c

redirection.o: redirection.c
		gcc $(CFLAGS) -c redirection.c

pipe.o: pipe.c
		gcc $(CFLAGS) -c pipe.c

clean:
		rm -f myshell *.o 