/* 
  Program -  ustawia ignorowanie sygnału, staje się liderem swojej
   grupy procesów, a następnie tworzy kilka procesów potomnych, które uruchamiają
   program  (prog_1.c).Wypisuje status zakonczenia potomkow.
 
 ----------------------------------------------------------------------
 Autor: Bartłomiej Kachnic,                             Krakow, 29.03.2022
 ----------------------------------------------------------------------
*/ 


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define PATH "./"
#define NUM_OF_PROCESS 5



int main(int argc, char *argv[])
{
    int stat_lock; // status potomka
    pid_t pid_p;
    if (argc!=4)
    {
    	perror("Wrong number of args(main)");
    	exit(EXIT_FAILURE);
    }

    if (setpgid(0, 0) == -1) // ustawienia procesu na lidera grupy
    {
        perror("setpgid error");
        exit(EXIT_FAILURE);
    } 
    
    printf("PGID lidera: %d\n", getpgrp());
    int i = atoi(argv[2]); // numer sygnalu

    if (signal(i, SIG_IGN) == SIG_ERR)  // ignorowanie sygnalu od macierzystego
        {
            perror("signal error");
            exit(EXIT_FAILURE);
        }
    char pathName[86];
    sprintf(pathName, "%s%s",PATH, argv[1]);
    int x; // iteracja petli
    for (x = 0; x < NUM_OF_PROCESS; x++)
    {
        switch(fork()) // tworzenie procesu potomnego
    {
        case -1:
            perror("fork error");
            exit(EXIT_FAILURE);
        case 0: // proces potomny
            
            execlp(pathName,argv[1], argv[2], argv[3], NULL); // wywolanie programu 1
    		    perror("execlp error");
    		    _exit(2);
        default:
            
            break;
           
            
            
    }
    }
    for (x = 0; x < NUM_OF_PROCESS; x++)
    {   
        pid_p = wait(&stat_lock);  // czekanie na proces
        if (pid_p == -1)
        {
            perror("wait error");
            exit(EXIT_FAILURE);
        }
        else
        {
        printf("Status zakonczego procesu o numerze %d wynosi %d!\n", pid_p, stat_lock);
        }
    }
    
    
            
   
    
    





    return 0;
}
