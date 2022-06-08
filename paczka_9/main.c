/*
========================================================================
Autor: Bartłomiej Kachnic,                           Krakow, 08.06.2022

    Program tworzy X watkow standardu POSIX, ktorych liczba jest podana
     w argumentach programu.Nastepenie wypisuje nr watku i nr 
     odpowiedniej sekcji prywatnej lub krytycznej, a takze licznik globalny.
      
========================================================================
*/

#define _REENTRANT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#define XRIGHT 100     // odleglosc wyswietlania po prawej stronie


pthread_mutex_t Mutex; 
int mutual_var; // wspolna zmienna
int y_down = -1; // o ile pozycji w dol
const int *n_sections_ptr;

//========================================================================
// funkcja gotoxy ustawia kursor na wspolrzednych x, y ( w terminalu);
void gotoxy(unsigned x, unsigned y)
{
  printf("\033[%d;%dH", y, x);
}
//========================================================================
// funkcja dla watkow
void * pthread_fun(void *id_pthread)
{
  sleep(4);
  int id = *(int *) id_pthread; // id watku
  int errnum; // kod bledu
  int private_var = 0; // prywatna zmienna
  
  int i; // iteracja petli
  int t; // czas w sekundach
  srand(id); // ustawienie losowego czasu w zal. od id
  t = rand() % 5 + 2; // losowy czas od 2 do 6 sekund
  
  for (i = 0; i < *n_sections_ptr; ++i)
  {
    printf("Nr watku %d i jego sekcji prywatnej %d!\n", id, i+1);
    y_down++;
    sleep(t); // czekanie
    
    
    errnum = pthread_mutex_lock(&Mutex);
    if (errnum)
    {
      printf("%s", strerror(errnum));
      pthread_exit(NULL);
    }
    //=======================================================
    // SEKCJA KRYTYCZNA:
    gotoxy(XRIGHT, 0); // przesuniecie kursora na prawo
    printf("Nr watku %d i nr sekcji krytycznej: %d, Licznik: %d!\n", id,i+1, mutual_var);
    printf("\033[%dB", y_down); // przesuniecie kursora  w dol o y_down pozycji

    private_var = mutual_var;
    private_var++;

    sleep(t); // czekanie
    mutual_var = private_var;

    errnum = pthread_mutex_unlock(&Mutex);
    if (errnum)
    {
      printf("%s", strerror(errnum));
      pthread_exit(NULL);
    }
    //=======================================================
  }
  free(id_pthread); // dealokacja pamieci
  pthread_exit(NULL);
}
//========================================================================

int main(int argc, char *argv[])
{
  int i; // iteracja petli
  int errnum; // kod bledu
  const int n_pthreads = atoi(argv[1]); // ilosc watkow
  const int n_sections = atoi(argv[2]); // ilosc sekcji prywatnych / krytycznych
  n_sections_ptr = &n_sections;

  errnum = pthread_mutex_init(&Mutex, NULL); //  Inicjalizacja Mutexu
  if (errnum)
  {
    printf("%s", strerror(errnum));
    exit(EXIT_FAILURE);
  }

  pthread_t pthread_ids[n_pthreads]; // tablica id watkow
  
  // Wypisanie adresu Mutexu
  printf("Adres Mutexu: %p!\n", (void*) &Mutex);
  
  
  
  for (i = 0; i < n_pthreads; ++i)
  {
    int *id = malloc(sizeof(int));
    *id = i+1;
    errnum = pthread_create(&pthread_ids[i],NULL, pthread_fun, (void*) id); // tworzenie watkow
    if (errnum)
    {
      printf("%s", strerror(errnum));
      exit(EXIT_FAILURE);
    }
    
    // Wypisanie nr watku i jego id
    printf("Watek nr %d o id %ld!\n", i+1, pthread_ids[i]);
  }
  
  for (i = 3; i > 0; --i)
  {
    printf("Czyszczenie ekranu za %d!\n", i);
    sleep(1);
  }
  if (system("clear") == -1)
  {
    perror("system clear error");
    exit(1);
  }
  for (i = 0; i < n_pthreads; ++i)
  {
    errnum = pthread_join(pthread_ids[i], NULL); // czekanie na watki
    if (errnum)
    {
      printf("%s", strerror(errnum));
      exit(EXIT_FAILURE);
    }
  }


  gotoxy(0, y_down+2); // przesuniecie kursora na dol

  if (mutual_var == n_pthreads*n_sections) // sprawdzenie wyniku
  {
    printf("Licznik jest poprawny i wynosi %d!\n", mutual_var);
  }
  else 
  {
    printf("Licznik jest niepoprawny i wynosi %d!\n", mutual_var);
  }
  
  pthread_mutex_destroy(&Mutex); // usuniecie mutexu

  return 0;
}