/*
========================================================================
Autor: Bartłomiej Kachnic,                           Krakow, 19.05.2022

    Konsument pobiera dane z pamieci dzielonej(bufora cyklicznego) i
    zapisuje je w pliku txt.
      
========================================================================
*/


#include <stdio.h>
#include <stdlib.h>
#include "sem_biblio.h"
#include "shm_biblio.h"
#define NKONS NELE - 1

int check_if_end(char * buff, int * run)
{
    for (int i = 0; i < NKONS; i++)
    {
        if (buff[i] == '\0') 
        {
            *run = 0;
            return i;
        }
    }
    return NKONS;
}

int main(int argc, char *argv[]) // 1- SEMAFOR PROD, 2-SEMAFOR KONS, 3-PAMIEC DZIELONA, 4 - WYNIKI.txt
{
    if (argc != 5)
    {
        printf("Wrong numer of args! Example: ./%s sem_1 sem_2 shared_memory data.txt", argv[0]);
        _exit(1);
    }

    sem_t *prod = otworz_semafor_nazwany(argv[1]); // adres semafora PROD
    sem_t *kons = otworz_semafor_nazwany(argv[2]); // adres semafora KONS
    int sm_fd = otworz_pamiec_dzielona(argv[3]); // deskryptor pamieci dzielonej

    //wypisanie nazwy semaforow i ich adresow
    printf("Konsument: %s %p, %s %p, Deskryptor pamieci dzielonej: %d!\n", argv[1], (void*) prod, argv[2], (void *) kons, sm_fd);
    
    Towar *towarKonsument = odzwzoruj_w_wirtualna_przestrzen_adr(sm_fd); 
    

    int fd; // deskryptor dla pliku
    fd = open(argv[4], O_WRONLY); // otwarcie pliku do pisania
    int ile_bajt; // ile odczytano
    towarKonsument->wyjmij = 0;
    int run = 1; // petla
    while(run)
    {
        P_sem_wait(kons); // opusc semafor konsumenta
        sleep(1);
        // STREFA KRYTYCZNA
        /*************************************************************************/
        ile_bajt = write(fd, towarKonsument->bufor[towarKonsument->wyjmij], check_if_end(towarKonsument->bufor[towarKonsument->wyjmij], &run)); // zapisanie do bufora
        
        if (ile_bajt == -1)
        {
            perror("read error Konsument");
            _exit(1);
        }
       
       //wypisanie wartosci semaforow
        printf("K: wartosc %s: %d, wartosc %s %d!\t", argv[1], pobierz_wartosc_semafora(prod), argv[2], pobierz_wartosc_semafora(kons));
        printf("K: Indeks elementu bufora: %d, liczba pobranych bajtow: %d, tekst: %s\n", towarKonsument->wyjmij, ile_bajt,towarKonsument->bufor[towarKonsument->wyjmij]);
        sleep(2);
        
        
        towarKonsument->wyjmij = (towarKonsument->wyjmij + 1) % NBUF; // zmiana pozycji w buforze
        
        /*************************************************************************/
        
        V_sem_post(prod); // podnies semafor Producenta
    } // end while

   
    
    if (close(fd) == -1) // zamknieciie pliku do pisania
        {
            perror("close file error (read)");
            _exit(1);
        }

    usun_odzwzorowanie_wirtualnej_przestrzeni(towarKonsument);
    zwolnij_zasoby_semafora(prod);
    zwolnij_zasoby_semafora(kons);
    zamknij_pamiec_dzielona(sm_fd);

    printf("Koniec pracy KONSUMENTA!\n");
    return 0;
}