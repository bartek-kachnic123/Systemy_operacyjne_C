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
#define NELE 5
#define NBUF 10

// struktura dla pamieci dzielonej Towar
typedef struct 
{
    char bufor[NBUF][NELE];
    int wstaw, wyjmij;
} Towar;


// shared memory

// tworzy pamiec dzielona o nazwie name i zwraca deskryptor
int stworz_pamiec_dzielona(const char *name);

// otwiera pamiec dzielona o nazwie name i zwraca deskryptor
int otworz_pamiec_dzielona(const char *name);

// ustawia dlugosc pamieci dzielonej o deskryptorze fd na lenght
void ustaw_dlugosc_pamieci_dzielone(int fd, off_t length);

// odwzorywuje strukture towar w wirtualna przestrzen pamieci dzielonej o deskryptorze fd i zwraca adres Towaru
Towar* odzwzoruj_w_wirtualna_przestrzen_adr(int fd);

// usuwa odzwzorowanie wirtualnej przestrzeni z pamieci dzielonej Towaru
void usun_odzwzorowanie_wirtualnej_przestrzeni(Towar *addr);

// zamyka pamiec dzielona o deskryptorze fd
void zamknij_pamiec_dzielona(int fd);

// usuwa pamiec dzielona o nazwie shm_name
void usun_pamiec_dzielona(const char *shm_name);


#endif