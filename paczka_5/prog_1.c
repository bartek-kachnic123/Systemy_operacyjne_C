/*
========================================================================
Autor: Bartłomiej Kachnic,                           Krakow, 27.04.2022

Program realizuje problem Producenta-Konsumenta za pomoca potokow nazwanych(FIFO).
       Proces producenta pobiera dane z pliku i umieszcza je w potoku,
       natomiast proces konsumenta(macierzysyty) odczytuje dane z potoku
       i wpisuje je do pliku.
========================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#define PATH "./"

/*******************************************************/
char *fifo; // globalna zmienna  do funkcji usun_potok
void usun_potok(void) // funkcja usuwajaca fifo przed zakonczeniem programu
{
  switch(unlink(fifo))
  {
    case -1:
      perror("unlink error");
      _exit(1);
    case 0:
      printf("Potok nazwany (FIFO) zostal pomyslnie usuniety!\n");
  }
  
}
/*******************************************************/

/*******************************************************/
void sig_handler(int sig)       // do obslugi sygnalu
{
  exit(0);
}
/*******************************************************/

int main(int argc, char *argv[]) // 1 - producent, 2 - konsument , 3- nazwa potoku fifo 4 - nazwa danych, 5 -  nazwa wynikow

{
    // sprawdzenie ilosci argumentow
    if (argc!=6)
      {
          perror("Wrong number of args (MAIN) ./ Example: ./prog_1 producent.x konsument.x fifo data.txt results.txt");
          _exit(EXIT_FAILURE);
      }
    
  
    // tworzenie fifo
    if (mkfifo(argv[3], 0644)  == -1)
    {
      perror("mkfifo error main");
      _exit(EXIT_FAILURE);
    }
   

    // rejestracja funkcji usun_potok
    fifo = argv[3]; // przypisanie wskaznika globalnego do nazwy fifo
    if (atexit(usun_potok) != 0)
    {
      perror("atexit error");
      exit(1);
    }
  
    // Wlasna obsluga sygnalu SIGINT
    if (signal(SIGINT, sig_handler) == SIG_ERR)
    {
      perror("signal error with sig_handler function");
      exit(1);
    }
    
    
    
    // tworzenie sciezek do producenta i klienta 
    char PATHNAME_PRODUCENT[86];
    char PATHNAME_KONSUMENT[86];
    sprintf(PATHNAME_PRODUCENT, "%s%s", PATH, argv[1]);
    sprintf(PATHNAME_KONSUMENT, "%s%s", PATH, argv[2]);
    
    /*******************************************************************************************/   
    // Obsluga procesu producenta
    pid_t producent; // pid producenta
    switch(producent = fork())
    {
      case -1:
        perror("fork error");
        exit(EXIT_FAILURE);

      case 0:
        execlp(PATHNAME_PRODUCENT,argv[1],argv[3], argv[4], NULL); // wywolanie programu Producent 
        perror("execlp error");
        _exit(2);
                  
      default:
        break;
    }
   /*******************************************************************************************/    
   

  /*******************************************************************************************/
  // Obsluga procesu konsumenta
   pid_t konsument; // pid producenta
    switch(konsument = fork())
    {
    case -1:
      perror("fork error");
      exit(EXIT_FAILURE);

    case 0:
      execlp(PATHNAME_KONSUMENT,argv[2],argv[3], argv[5],  NULL); // wywolanie programu  Konsument
      perror("execlp error");
      _exit(2);
                
    default:
    break;
    }
    /*******************************************************************************************/

    // czekanie na proces producenta
    if (waitpid(producent, NULL, 0) == -1)
    {
      perror("waitpid producent process error");
      exit(1);
    }
    
    // czekanie na proces konsumenta
    if (waitpid(konsument, NULL, 0) == -1)
    {
      perror("waitpid konsument process error");
      exit(1);
    }
  
    exit(0);
}