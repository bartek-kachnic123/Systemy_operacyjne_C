/* 
 Program wywołuje funkcję fork 3 razy i wypisuje identikatory (UID, GID, PID, PPID, PGID)
 procesu macierzystego oraz wszystkich procesow potomnych. Za pomoca funkcji sleep proces
 macierzysty kończy się pierwszy, a sieroty(pozostałe procesy potomne) zostają zaadoptowane
 przez systemd/init.
 
 ----------------------------------------------------------------------
 Autor: Bartłomiej Kachnic,                             Krakow, 12.03.2022
 ----------------------------------------------------------------------
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


//=========================================================================
void wypisz_id()
{
 printf("\nUID: %u GID: %u PID: %u PPID: %u PGID: %u \n====================================================\n",getuid(), getgid(), getpid(),getppid(), getpgrp());
}
//=========================================================================

int main()
{
	int i; // iteracja petli
	printf("=================\n");
	printf("Proces macierzysty: \n");
	wypisz_id();                     // proces macierzysty
	for ( i = 0; i < 3; ++i)
	{
		switch(fork())  // tworzenie nowych procesow
		{
		
		case -1:
			perror("fork error");
			exit(1);
		
		case 0:
      		sleep(4);  //czekanie
		 	wypisz_id();
     		sleep(4);  //czekanie
		 	
		default: break;
			
		}	
		
	}
	
	
	exit(0);
}


