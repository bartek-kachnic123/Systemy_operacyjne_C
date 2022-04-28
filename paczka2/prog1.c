/* 
 Program wywoluje funkcje fork 3 razy. Wypisuje wartosci UID,GID,PID,PPID,PGID
 dla procesu macierzystego i procesow potomnych. Main odbiera argument,
 ktory jest nazwa funkcji do wywolania przez excelp().
 ----------------------------------------------------------------------
 Autor: Bartłomiej Kachnic,                             Krakow, 19.03.2022
 ----------------------------------------------------------------------
*/ 


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#define PATH "./"


int main(int argc, char *argv[])
{
    if (argc!=2)
    {
    	perror("Niepoprawna liczba argumentow main");
    	exit(1);
    }
    int i; // iteracja for
    char pathName[86];
    sprintf(pathName, "%s%s",PATH, argv[1]); 
    
    printf("Proces macierzysty: ");
    printf("UID: %u GID: %u PID: %u PPID: %u PGID: %u \n",getuid(), getgid(), getpid(),getppid(), getpgrp()); 
    for (i = 0; i < 3; ++i)
    {
	    switch(fork())
  		{
  		case -1:
  		perror("fork error");
  		exit(EXIT_FAILURE);
     
  		case 0: // proces potomny
  		execlp(pathName,argv[1],NULL); // wywolanie 2 programu
  		perror("execlp error");
  		_exit(2);
     
  		default: 
  			if (wait(NULL) == -1 || errno == ECHILD) // czekanie na zakonczenie procesu potomnego
  			{
  				perror("Proces nie ma potomnych procesow!");
          		exit(EXIT_FAILURE);
  			}
  	    }
    }






    return 0;
}
