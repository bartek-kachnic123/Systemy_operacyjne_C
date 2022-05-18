/*
========================================================================
Autor: Bartłomiej Kachnic,                           Krakow, 11.05.2022

    Biblioteka do obslugi bledow funkcji semaforow.
========================================================================
*/


#if !defined(SEM_AND_SHARED_MEM_BIB)
#define SEM_AND_SHARED_MEM_BIB

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#define NELE 10
#define NBUF 5

typedef struct 
{
    char bufor[NELE][NBUF];
    int wstaw, wyjmij;
} Towar;



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

// shared memory
int stworz_pamiec_dzielona(const char *name);
int otworz_pamiec_dzielona(const char *name);
void ustaw_dlugosc_pamieci_dzielone(int fd, off_t length);
Towar* odzworuj_w_wirtualna_przestrzen_adr(int fd);
void usun_odzwzorowanie_wirtualnej_przestrzeni(Towar *addr);
void zamknij_pamec_dzielona(int fd);
void usun_pamiec_dzielona(const char *shm_name);

#endif // SEM_AND_SHARED_MEM_BIB