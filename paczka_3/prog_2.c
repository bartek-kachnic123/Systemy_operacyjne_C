/* 
  Program -  w procesie potomnym uruchamia program (prog_1.c), po
   czy z procesu macierzystego wysyła do niego sygnał.
 
 ----------------------------------------------------------------------
 Autor: Bartłomiej Kachnic,                             Krakow, 29.03.2022
 ----------------------------------------------------------------------
*/ 


#define _POSIX_C_SOURCE 200112L
#define PATH "./"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>



int main(int argc, char *argv[]) // argv[1] = program do exec, argv[2] = numer syganlu, argv[3] = wybor akcji
{
    if (argc!=4)
    {
    	perror("Wrong number of args(main)");
    	exit(EXIT_FAILURE);
    }
    char pathName[86];
    sprintf(pathName, "%s%s",PATH, argv[1]);
    
    int i = atoi(argv[2]); // numer sygnalu
    pid_t pid1;
    switch(pid1 = fork()) // tworzenie procesu potomnego
    {
        case -1:
            perror("fork error");
            exit(EXIT_FAILURE);
        case 0: // proces potomny
            execlp(pathName,argv[1], argv[2], argv[3], NULL); // wywolanie 2 programu
		    perror("execlp error");
		    _exit(2);
        default:
            sleep(2);
            if (kill(pid1, 0) == -1)  // sprawdzenie czy proces potomny istnieje
            {
                perror("Process doesnt exist");
                exit(EXIT_FAILURE);
            }
            
            
            
            if (kill(pid1, i) == -1) // wyslanie sygnalu i
            {
                perror("kill error");
                exit(EXIT_FAILURE);
            }     
            if (wait(NULL) == -1)  // czekanie na zakonczenie procesu
            {
                perror("wait error");
                exit(EXIT_FAILURE);
            }
            
            
            

    }





    return 0;
}
