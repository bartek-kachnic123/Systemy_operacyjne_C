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
#define XLEFT 0        // wyswietlanie po lewej stronie
#define CLEAR_TIME 5 // czas do wyczyszczenia ekranu


pthread_mutex_t Mutex;  // Mutex do synchronizacji
int mutual_var; // wspolna zmienna
const int *n_sections_ptr;
const int *n_pthreads_ptr;

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
  sleep(CLEAR_TIME+1);
  int id = *(int *) id_pthread; // id watku
  int errnum; // kod bledu
  int private_var = 0; // prywatna zmienna
  int posY; // pozycja kursora dla dlugosci Y
  
  int i; // iteracja petli
  int t; // czas w sekundach
  srand(id); // ustawienie losowego czasu w zal. od id
  t = rand() % 4 + 3; // losowy czas od 3 do 6 sekund
  
  for (i = 0; i < *n_sections_ptr; ++i)
  {
    posY = (i * 4) + id; // obliczanie wartosci Y na podstawie id i ilosci watkow

    sleep(t); // czekanie
    gotoxy(0, posY);
    printf("Nr watku %d i jego sekcji prywatnej %d!\n", id, i+1);
    
    errnum = pthread_mutex_lock(&Mutex);
    if (errnum)
    {
      printf("%s", strerror(errnum));
      free(id_pthread);
      pthread_exit(NULL);
    }
    //=======================================================
    // SEKCJA KRYTYCZNA:
    
    private_var = mutual_var;
    private_var++;

    t = rand() % 5 + 1; // losowy czas od 1 do 5 sekund
    sleep(t); // czekanie
    mutual_var = private_var;

    gotoxy(XRIGHT, posY);
    printf("Nr watku %d i nr sekcji krytycznej: %d, Licznik: %d!\n", id,i+1, mutual_var);

    errnum = pthread_mutex_unlock(&Mutex);
    if (errnum)
    {
      printf("%s", strerror(errnum));
      free(id_pthread);
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
  n_pthreads_ptr = &n_pthreads;

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
  
  for (i = CLEAR_TIME; i > 0; --i) // odliczanie
  {
    printf("Czyszczenie ekranu za %d!\n", i);
    sleep(1);
  }

  if (system("clear") == -1) // czyszczenie ekranu
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

  gotoxy(XRIGHT/2, n_sections*n_pthreads+1); // przesuniecie kursora na dolny srodek

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