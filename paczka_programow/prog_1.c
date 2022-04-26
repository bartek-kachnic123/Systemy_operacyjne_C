/* 
 Program wypisuje wartości (UID, GID, PID, PPID, PGID) dla danego procesu.
 ----------------------------------------------------------------------
 Autor: Bartłomiej Kachnic,                             Krakow, 12.03.2022
 ----------------------------------------------------------------------
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>



int main()
{
	printf("UID: %u GID: %u PID: %u PPID: %u PGID: %u \n",getuid(), getgid(), getpid(),getppid(),getpgrp()); 
 
	return 0;
}

