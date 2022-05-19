/*
========================================================================
Autor: Bartłomiej Kachnic,                           Krakow, 27.04.2022


========================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include "semafor_biblio.h"
#define PATH "./"
#define SEM "semafor_1"
#define VSEM 1

/*******************************************************************/
void sig_handler(int num) // funkcja obslugujaca SIGINT
{
  exit(0);
}
/*******************************************************************/
int main(int argc, char *argv[]) // argv[1] - nazwa programu, argv[2] - liczba procesow, argv[3] - numer.txt
{ 
  name_semaphore = SEM; // przypisanie nazwy semafora do funkcji zamykajacej
  if (atexit(usun_semafor) != 0) // funkcja usuwajacego semafor przed zakonczeniem programu
  {
    perror("atexit error");
    _exit(EXIT_FAILURE);
  }

  if (argc != 4) // sprawdzenie poprawnosci liczby argumentow
  {
    perror("wrong number of args! Example: ./powielacz name_of_programs num_of_process, number_of_critical_sections");
    _exit(EXIT_FAILURE);
  }

  if (signal(SIGINT, sig_handler) == SIG_ERR) // sygnal osblugujacy SIGINT
  {
    perror("SIGINT signal error");
    _exit(EXIT_FAILURE);
  }

  FILE *fp = fopen(argv[3], "w"); // stworzenie pliku numer.txt
  int value = 0; // wartosc wpisana do pliku

  if (!fp) // sprawdzenie czy udalo sie otworzyc plik
  {
    perror("cant open file to write");
    _exit(EXIT_FAILURE);
  }

  if (fprintf(fp, "%d", value) < 0) // wpisanie do pliku wartosci value
  {
    perror("fprintf error");
    _exit(EXIT_FAILURE);
  }

  if (fclose(fp) == -1)          // zamkniecie pliku txt
  {
    perror("fclose error with file");
    _exit(1);
  }

  sem_t *sem = utworz_semafor_nazwany(SEM, VSEM); // tworzenie semafora
  printf("Adres %s = %p i wartosc = %d!\n", SEM, (void*) sem, VSEM);

  
  int i; // iteracja petli
  int num_of_process = atoi(argv[2]); // liczba procesow
  char num[20]; // numer sekcji

  char pathName[86]; // sciezka do programu 
  sprintf(pathName, "%s%s", PATH, argv[1]);

  for ( i = 0; i < num_of_process; i++)
  {
    switch(fork())
    {
      case -1:
        perror("fork error");
        exit(EXIT_FAILURE);
      
      case 0:
        sprintf(num, "%d", i+1);
        execlp(pathName, argv[1],SEM,argv[3], num ,  NULL); // argv[1] - nazwa programu, argv[2] - nazwa semafora, argv[3] - nazwa pliku txt argv[4] - numer sekcji 
        perror("excelp errror");
        _exit(2);
    }
  } // end for


  for (i = 0; i < num_of_process; i++) // czekanie na procesy
  {
    if (wait(NULL) == -1)
    {
      perror("wait error");
      exit(EXIT_FAILURE);
    }
  }
  
  
  fp = fopen(argv[3], "r"); // otwarcie pliku do czytania

  if (!fp) // czy udalo sie otworzyc plik
  {
    perror("cant open file to read");
    exit(EXIT_FAILURE);
  }

  if (fscanf(fp, "%d", &value) < 0) // odczytanie wartosci z pliku
  {
    perror("cant read file");
    exit(EXIT_FAILURE);
  }

  if (fclose(fp) == -1) // zamkniecie pliku do czytania
  {
      perror("fclose error with file");
      exit(1);
  }
  
  if (value == num_of_process) // sprawdzenie poprawnosci wartosci liczby z pliku txt
  {
    printf("Wartosc wpisana w pliku jest poprawna i wynosi %d!\n", value);
  }
  else
  {
    printf("Wartosc jest niepoprawna i wynosi: %d!\n", value);
  }

  exit(0);
}
