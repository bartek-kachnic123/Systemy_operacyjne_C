#define _REENTRANT
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define NUM_OF_THREAD 3 // dodac do arg programow
#define NUM_SECTIONS 5 // to tez

int mutual_var; // wspolna zmienna

struct Pthread_args
{
  pthread_mutex_t *mutex;
  unsigned num_sections;
  unsigned num_pth;
};


void gotoxy(unsigned x, unsigned y)
{
  printf("\033[%d;%dH\033[2K", y, x);
}
void * pthread_fun(void *pthread_args)
{

  struct Pthread_args *args = (struct Pthread_args *) pthread_args;
  int private_var = 0;
  pthread_mutex_lock(args->mutex);

  //=======================================================
  // SEKCJA KRYTYCZNA:
  private_var = mutual_var;
  private_var++;
  sleep(2);
  mutual_var = private_var;
  printf("Nr watku %d!\n", args->num_pth);
  
  pthread_mutex_unlock(args->mutex);
  //=======================================================
  pthread_exit(NULL);
}

int main()
{
  pthread_mutex_t Mutex = PTHREAD_MUTEX_INITIALIZER; // stworzenie i inicjalizacja mutexu
  int i; // iteracja petli
  pthread_t pthread_ids[NUM_OF_THREAD]; // tablica id watkow
  
  // Wypisanie adresu Mutexu
  printf("Adres Mutexu: %p!\n", (void*) &Mutex);
  // tworzenie struktrury dla argumentow watkow i ich inizalizacja
  static struct Pthread_args pth_args;
  pth_args.mutex = &Mutex;
  pth_args.num_sections = NUM_SECTIONS;

  for (i = 0; i < NUM_OF_THREAD; ++i)
  {
    pth_args.num_pth = i + 1;
    pthread_create(&pthread_ids[i],NULL, pthread_fun, (void *)&pth_args);

    // Wypisanie nr watku i jego id
    printf("Watek nr %d o id %ld!\n", pth_args.num_pth, pthread_ids[i]);
  }
  for (i = 0; i < NUM_OF_THREAD; ++i)
  {
    pthread_join(pthread_ids[i], NULL); // czekanie na watki
  }


  pthread_mutex_destroy(&Mutex); // usuniecie mutexu





  return 0;
}