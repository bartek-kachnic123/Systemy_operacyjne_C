/* 
 Program wywołuje funkcję fork 3 razy i wypisuje identikatory (UID, GID, PID, PPID, PGID)
 procesu macierzystego oraz wszystkich procesow potomnych. Program wyświetla je
 w kolejności od najstarszego do najmłodszego.
 
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
void wypisz_id()      // Wypisuje UID, GID, PID, PPID, PGID
{
   printf("UID: %u GID: %u PID: %u PPID: %u PGID: %u \n====================================================\n",getuid(), getgid(), getpid(),getppid(), getpgrp());
}

//=========================================================================



int main()
{
	int i; // iteracja petli
	printf("=================\n");
	printf("Proces macierzysty: \n");
	wypisz_id();                // proces macierzysty
	printf("Procesy potomne: \n");
	for ( i = 0; i < 3; ++i)
	{
			switch(fork())  // tworzenie nowych procesow
			{
			
			case -1:
				perror("fork error");
				exit(1);
			
			case 0:                        
			 	wypisz_id();
        		sleep(1);                             
        
			 	break;
			 	
			default:
        
				  
				break;	
			}		
	}
	
	
	sleep(5);
	return 0;
}


