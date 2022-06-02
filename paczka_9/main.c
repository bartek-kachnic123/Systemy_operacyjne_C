#define _REENTRANT
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define NUM_OF_THREAD 5 // dodac do arg programow
#define NUM_SECTIONS 5 // to tez
#define XMAX 100     // Maksymalna liczba kolumn ekranu 
#define YMAX 50     // Maksymalna liczba wierszy ekranu


int mutual_var; // wspolna zmienna

typedef struct
{
  pthread_mutex_t *mutex;
  unsigned num_sections;
  int id;
}Pthread_args;


void gotoxy(unsigned x, unsigned y)
{
  
  printf("\033[%d;%dH\033[", y, x);
}
void * pthread_fun(void *pthread_args)
{
  sleep(4);
  Pthread_args *args = (Pthread_args *) pthread_args;
  int private_var = 0; // prywatna zmienna
  
  int i; // iteracja petli
  for (i = 0; i < args->num_sections; ++i) // sekcja prywatna
  {
    
    printf("Nr watku %d i jego sekcji prywatnej %d!\n", args->id, i+1);
    sleep(1);
  }
  for (i = 0; i < args->num_sections; ++i)
  {
    // gotoxy(1, i*3);
    // printf("NNr watku %ld i jego sekcji prywatnej %d!\n", num_pth, i+1);
    

    pthread_mutex_lock(args->mutex);
    //=======================================================
    // SEKCJA KRYTYCZNA:
    private_var = mutual_var;
    private_var++;
    sleep(2);
    mutual_var = private_var;
    gotoxy(XMAX, 0);
    printf("NNr watku %d i nr sekcji krytycznej: %d!\n", args->id,i+1);
    
    
    pthread_mutex_unlock(args->mutex);
    
    
    //=======================================================
  }
  free(args); // dealokacja pamieci
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
  
  
  for (i = 0; i < NUM_OF_THREAD; ++i)
  {
    Pthread_args *ptr_args = malloc(sizeof(Pthread_args));
    ptr_args->mutex = &Mutex;
    ptr_args->num_sections = NUM_SECTIONS;
    ptr_args->id = i+1;


    pthread_create(&pthread_ids[i],NULL, pthread_fun, (void*)ptr_args);
    
    // Wypisanie nr watku i jego id
    printf("Watek nr %d o id %ld!\n", i+1, pthread_ids[i]);
  }
  sleep(3);
  if (system("clear") == -1)
  {
    perror("system clear error");
    exit(1);
  }
  for (i = 0; i < NUM_OF_THREAD; ++i)
  {
    pthread_join(pthread_ids[i], NULL); // czekanie na watki
  }

  gotoxy(0, YMAX); // przesuniecie kursora na dol
  if (mutual_var == NUM_OF_THREAD*NUM_SECTIONS)
  {
    printf("LLicznik jest poprawny i wynosi %d!\n", mutual_var);
  }
  else 
  {
    printf("LLicznik jest niepoprawny i wynosi %d!\n", mutual_var);
  }
  
  pthread_mutex_destroy(&Mutex); // usuniecie mutexu





  return 0;
}