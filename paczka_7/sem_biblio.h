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

// tworzy semafor nienazwany o adresie sem, deskryptorze pshared i wartosci value
void utworz_semafor_nienazwany(sem_t *sem, int pshared, unsigned int value);

// usuwa semafor nienazwany o adresie sem
void usun_semafor_nienazwany(sem_t *sem);

// tworzy semafor nazwany o nazwie name i wartosci value. Zwraca adres semafora
sem_t *utworz_semafor_nazwany(const char *name, int value);

// otwiera semafor nazwany o nazwie name i zwraca adres semafora
sem_t *otworz_semafor_nazwany(const char *name);

// zwalnia zasoby semafora o adresie sem
void zwolnij_zasoby_semafora(sem_t *sem);

// usuwa semafor o nazwie name
void usun_semafor(const char *name);

// podnies semafor o adresie sem
void V_sem_post(sem_t *sem);

// opusc semafor o adresie sem
void P_sem_wait(sem_t *sem);

//zwraca wartosc semafora o adresie sem
int pobierz_wartosc_semafora(sem_t *sem);


#endif