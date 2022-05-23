#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sem_biblio.h"
#include "shm_biblio.h"
#define NPROD 3

int main(int argc, char *argv[]) // 1- SEMAFOR PROD, 2-SEMAFOR KONS, 3-PAMIEC DZIELONA, 4 - DANE.txt
{
    if (argc != 5)
    {
        printf("Wrong numer of args! Example: ./%s sem_1 sem_2 shared_memory data.txt", argv[0]);
        _exit(1);
    }
    sem_t *prod = otworz_semafor_nazwany(argv[1]); // adres semafora PROD
    sem_t *kons = otworz_semafor_nazwany(argv[2]); // adres semafora KONS
    int sm_fd = otworz_pamiec_dzielona(argv[3]); // deskryptor pamieci dzielonej

    printf("Producent: %s %p, %s %p, Deskryptor pamieci dzielonej: %d!\n", argv[1], (void*) prod, argv[2], (void *) kons, sm_fd);
    Towar *towarProducent = odzworuj_w_wirtualna_przestrzen_adr(sm_fd);
    if (towarProducent == MAP_FAILED)
    {
        perror("mmap error Konsument");
        _exit(1);
    }

    int fd; // deskryptor dla pliku
    fd = open(argv[4], O_RDONLY); // otwarcie pliku do czytania
    int ile_bajt; // ile bajtow pobrano
    while(1)
    {
        P_sem_wait(prod); // opusc semafor Producenta
        // STREFA KRYTYCZNA
        /*************************************************************************/
        sleep(1);
        ile_bajt = read(fd, towarProducent->bufor[towarProducent->wstaw], NPROD); // zapisanie do bufora
       
        if (ile_bajt == -1)
        {
            perror("read error Producent");
            _exit(1);
        }

        if (ile_bajt == 0)
        {
            printf("Koniec pracy Producenta!\n");
            towarProducent->bufor[towarProducent->wstaw][0] = '\0'; // przeslanie znaku konca pliku
            break;
        }
        printf("P: wartosc %s: %d, wartosc %s %d!\t", argv[1], pobierz_wartosc_semafora(prod), argv[2], pobierz_wartosc_semafora(kons));
        printf("P: Indeks elementu bufora: %d, liczba wstawionych bajtow: %d, tekst: %s\n", towarProducent->wstaw, ile_bajt,towarProducent->bufor[towarProducent->wstaw]);
        sleep(1);
        
        towarProducent->wstaw = (towarProducent->wstaw + 1) % NELE; // zmiana pozycji w buforze
       
       
        /*************************************************************************/
        V_sem_post(kons); // podnies semfaor Konsumenta

        
    } // end while

     if (close(fd) == -1) // zamkniecie  pliku do czytania
        {
            perror("close file error (read)");
            _exit(2);
        }
    
    usun_odzwzorowanie_wirtualnej_przestrzeni(towarProducent);
    zwolnij_zasoby_semafora(prod);
    zwolnij_zasoby_semafora(kons);
    zamknij_pamiec_dzielona(sm_fd);
    
    return 0;
}