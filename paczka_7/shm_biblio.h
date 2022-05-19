/*
========================================================================
Autor: Bartłomiej Kachnic,                           Krakow, 19.05.2022

    Biblioteka do obslugi bledow funkcji pamieci dzielonej.
========================================================================
*/


#if !defined(SHM_BIBLIO)
#define SHM_BIBLIO

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


// shared memory
int stworz_pamiec_dzielona(const char *name);
int otworz_pamiec_dzielona(const char *name);
void ustaw_dlugosc_pamieci_dzielone(int fd, off_t length);
Towar* odzworuj_w_wirtualna_przestrzen_adr(int fd);
void usun_odzwzorowanie_wirtualnej_przestrzeni(Towar *addr);
void zamknij_pamiec_dzielona(int fd);
void usun_pamiec_dzielona(const char *shm_name);


#endif