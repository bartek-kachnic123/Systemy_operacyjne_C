/* 
  Program - tworzy jeden proces potomny i uruchamia w nim program
   (prog_3_pomoc.c), który ustawia ignorowanie sygnału, staje się liderem swojej
   grupy procesów, a następnie tworzy kilka procesów potomnych, które uruchamiają
   program  (prog_1.c).Pierwszy proces macierzysty  wysyła sygnał do całej grupy.
 
 ----------------------------------------------------------------------
 Autor: Bartłomiej Kachnic,                             Krakow, 29.03.2022
 ----------------------------------------------------------------------
*/ 

#define _POSIX_C_SOURCE 200112
#define PATH "./"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>



int main(int argc, char *argv[]) // 1 - pierwszy program 2 - drugi program 3 - numer sygnalu 4 - wybrana opcja
{
     if (argc!=5)
    {
    	perror("Wrong number of args(main)");
    	exit(EXIT_FAILURE);
    }
    pid_t pid = 0; // przechowanie pid procesu potomnego
    char pathName[86];
    sprintf(pathName, "%s%s",PATH, argv[1]);
    
    int i = atoi(argv[3]); // numer sygnalu
    
    switch(pid=fork()) // tworzenie procesu potomnego i zapamietanie pid procesu potomnego
    {
        case -1:
            perror("fork error");
            exit(EXIT_FAILURE);
            
        case 0: // proces potomny
            execlp(pathName,argv[1], argv[2], argv[3], argv[4], NULL); // wywolanie 2 programu
		        perror("execlp error");
		        _exit(2);
                    
        default:
            sleep(2); // czekanie
            
            if (kill(-pid, 0) == -1) // czy proces istnieje
              {
                  perror("Process doesnt exist");
                  exit(EXIT_FAILURE);
              }
              
                    
            if (kill(-pid, i) == -1) // wyslanie sygnalu i
            {
              perror("kill error");
              exit(EXIT_FAILURE);
            }
            if(wait(NULL) == -1)
            {
              perror("wait error");
              exit(EXIT_FAILURE);
            }
            break;
            
            
            
            
            

    }
    
    
    





    return 0;
}
