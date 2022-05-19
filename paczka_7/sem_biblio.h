/*
========================================================================
Autor: Bartłomiej Kachnic,                           Krakow, 11.05.2022

    Biblioteka do obslugi bledow funkcji semaforow.
========================================================================
*/


#if !defined(SEM_BIBLIO)
#define SEM_BIBLIO

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>

// semaphore 
void utworz_semafor_nienazwany(sem_t *sem, int pshared, unsigned int value); 
void usun_semafor_nienazwany(sem_t *sem);
sem_t *utworz_semafor_nazwany(const char *name, int value);
sem_t *otworz_semafor_nazwany(const char *name);
void zwolnij_zasoby_semafora(sem_t *sem);
void usun_semafor(const char *name);
void V_sem_post(sem_t *sem);
void P_sem_wait(sem_t *sem);
int pobierz_wartosc_semafora(sem_t *sem);


#endif