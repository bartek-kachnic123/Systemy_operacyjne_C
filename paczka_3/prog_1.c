/* 
 Program - ustawia obsługę sygnału na 3 sposoby zgodnie z 
  opcją podaną jako argument wywołania programu.
  1 - Domyslna akcja sygnalu
  2 - Zignorowanie sygnalu
  3 - Wlasna obsluga sygnalu
 ----------------------------------------------------------------------
 Autor: Bartłomiej Kachnic,                             Krakow, 29.03.2022
 ----------------------------------------------------------------------
*/ 




#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
 

extern const char * const sys_siglist[]; // lista sygnalow

/***********************************************************************/
void my_sighandler(int sig)   // Wypisuje numer i nazwe sygnalu
{
    printf("Numer sygnalu: %d Nazwa sygnalu: %s\n", sig, sys_siglist[sig]);
    sleep(1);
    
}
/***********************************************************************/
int main(int argc, char *argv[]) // 1 - numer sygnalu, 2 - wybrana opcja
{
    if (argc!=3)
    {
        perror("wrong number of arguments(main)");
        exit(EXIT_FAILURE);
    }
    int i = atoi(argv[1]); // numer sygnalu
    int j = atoi(argv[2]); // wybrana opcja
    printf("PID procesu: %u\n", getpid());  // PID procesu
    if (j == 1)             // domyslna 
    {
        
        if (signal(i, SIG_DFL) == SIG_ERR)  
        {
            perror("signal error");
            exit(EXIT_FAILURE);
        }
        pause(); // czekanie na sygnal
       
    }
    if (i == SIGSTOP || i == SIGKILL)
    {
        perror("Uzyto SIGSTOP lub SIGKILL");
        exit(1);
    }

    if (j == 2)             // ignorowanie
    {
        
        if (signal(i, SIG_IGN) == SIG_ERR)  
        {
            perror("signal error");
            exit(EXIT_FAILURE);
        }
        sleep(10); // czekanie na sygnal
        printf("Sygnal zostal zignorowany.\n");
        sleep(1);

        
    }

    if (j == 3)             // wlasna obsluga sygnalu
    {
        
        if (signal(i, my_sighandler) == SIG_ERR)  
        {
            perror("signal error");
            exit(EXIT_FAILURE);
        }
        pause(); // czekanie na sygnal
        
        
    }

    


    
    
    return 0;
}
