/*
========================================================================
Autor: Bartłomiej Kachnic,                           Krakow, 14.06.2022

    Program realizuje wzajmne wykluczanie P-watkow standardu POSIX
    za pomoca algorytmu piekarni Leslie Laporta.
      
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
#define N_THREADS 5  // ilosc watkow
#define N_SECTIONS 4 // ilosc sekcji

int mutual_var; // wspolna zmienna
volatile int number[N_THREADS];
volatile int choosing[N_THREADS];



//========================================================================
// funkcja gotoxy ustawia kursor na wspolrzednych x, y ( w terminalu);
void gotoxy(unsigned x, unsigned y)
{
  printf("\033[%d;%dH", y, x);
}
//========================================================================
// funkcja zwracajaca najwieksza wartosc z tablicy o rozmiarze num
int max_num(volatile int *tab, int num)
{
  int number = 0;
  int i = 0; // iteracja
  while (i!=num)
  {
    if (number < tab[i])
    {
      number = tab[i];
    }
    i++;
  }
  return number;
}
//========================================================================
// funkcja porownojaca wartosci (a,b) < (c,d) i zwracja true/false
int compare_n(volatile int *num, const int i, const int j)
{
  if (num[j] < num[i]) return 1;
  else if (num[j] == num[i] && j < i) return 1;
  else return 0;
}
//========================================================================
// funkcja dla watkow
void * pthread_fun(void *id_pthread)
{
  sleep(CLEAR_TIME+1);
  int id = *(int *) id_pthread; // id watku
  
  int private_var = 0; // prywatna zmienna
  int posY; // pozycja kursora dla dlugosci Y
  
  int i; // iteracja petli
  int t; // czas w sekundach
  srand(id); // ustawienie losowego czasu w zal. od id
  t = rand() % 4 + 3; // losowy czas od 3 do 6 sekund
  
  for (i = 0; i < N_SECTIONS; ++i)
  {
    posY = (i * N_THREADS) + id +1; // obliczanie wartosci Y na podstawie id i ilosci watkow

    choosing[id] = 1;
    number[id] = max_num(number, N_THREADS) + 1;
    choosing[id] = 0;

    sleep(t); // czekanie
    gotoxy(0, posY);
    printf("Nr watku %d i jego sekcji prywatnej %d!\n", id+1, i+1);

    for (int j = 0; j < N_THREADS; j++)
    {
      while(choosing[j]); // czekanie
      while(number[j] != 0 && compare_n(number, id, j)); // czekanie
    }
    
    //=======================================================
    // SEKCJA KRYTYCZNA:
    private_var = mutual_var;
    private_var++;

    t = rand() % 5 + 1; // losowy czas od 1 do 5 sekund
    sleep(t); // czekanie
    mutual_var = private_var;

    gotoxy(XRIGHT, posY);
    printf("Nr watku %d i nr sekcji krytycznej: %d, Licznik: %d!\n", id+1,i+1, mutual_var);
    number[id] = 0;

    //=======================================================
  } // endfor
  free(id_pthread); // dealokacja pamieci
  pthread_exit(NULL);
}
//========================================================================

int main(int argc, char *argv[])
{
  int i; // iteracja petli
  int errnum; // kod bledu

  pthread_t pthread_ids[N_THREADS]; // tablica id watkow
  
  for (i = 0; i < N_THREADS; ++i)
  {
    int *id = malloc(sizeof(int));
    *id = i;
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

  for (i = 0; i < N_THREADS; ++i)
  {
    errnum = pthread_join(pthread_ids[i], NULL); // czekanie na watki
    if (errnum)
    {
      printf("%s", strerror(errnum));
      exit(EXIT_FAILURE);
    }
  }

  gotoxy(XRIGHT/2, N_SECTIONS*N_THREADS+1); // przesuniecie kursora na dolny srodek

  if (mutual_var == N_SECTIONS*N_THREADS) // sprawdzenie wyniku
  {
    printf("Licznik jest poprawny i wynosi %d!\n", mutual_var);
  }
  else 
  {
    printf("Licznik jest niepoprawny i wynosi %d!\n", mutual_var);
  }

  return 0;
}