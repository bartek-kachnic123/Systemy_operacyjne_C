/*
========================================================================
Autor: Bartłomiej Kachnic,                           Krakow, 19.05.2022

    Program realizuje problem Konsumenta-Producenta. Tworzy 2 procesy
    potomne, z ktorych Producent pobiera dane z pliku dane.txt i zapisuje je
    w utworzonej przez program glowny pamieci dzielonej(bufor cykliczny).
    Natomiast drugi proces Konsument pobiera dane z pamieci i zapisuje
    je do pliku wyniki.txt
      
========================================================================
*/


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include "sem_biblio.h"
#include "shm_biblio.h"
#define SHAR_MEM "/shared_memory"
#define PROD "prod_1"
#define KONS "kons_1"
#define VPROD NELE
#define VKONS 0
#define PATH "./"

//========================================================================//
void sig_handler(int sig) // funkcja do obslugi sygnalu SIGINT
{
    exit(0);
}
//========================================================================//
void exit_handler() // funkcja wykonujaca sie przed zakonczeniem programu
{
    usun_semafor(PROD);
    usun_semafor(KONS);
    usun_pamiec_dzielona(SHAR_MEM);
}
//========================================================================//

int main(int argc, char *argv[]) // 1-producent, 2-konsument, 3-dane, 4- wyniki
{
    if (argc != 5)
    {
        printf("Wrong numer of args! Example: ./%s producent.x konsument.x data.txt results.txt", argv[0]);
        _exit(1);
    }

    if (atexit(exit_handler) != 0) // rejestracja funkcji atexit
    {
        perror("atexit function error");
        _exit(2);
    }
    
    sem_t *prod = utworz_semafor_nazwany(PROD, VPROD); // semafor do wstrzymania producenta
    sem_t *kons = utworz_semafor_nazwany(KONS, VKONS); // semafor do wstrzymania konsumenta
    
    printf("Semafor %s o adresie %p i wartosci %d!\n", PROD, (void *) prod, pobierz_wartosc_semafora(prod));
    printf("Semafor %s o adresie %p i wartosci %d!\n", KONS, (void *) kons, pobierz_wartosc_semafora(kons));

    int sm_fd = stworz_pamiec_dzielona(SHAR_MEM); // stworzenie pamieci dzielonej
    ustaw_dlugosc_pamieci_dzielone(sm_fd, sizeof(Towar)); // ustawienie dlugosci
    
    printf("Pamiec dzielona o deskryptorze %d i rozmiarze %ld!\n", sm_fd, sizeof(Towar));

    if (signal(SIGINT, sig_handler) == SIG_ERR)  // obsluga sygnalu SIGINT
    {
        perror("signal sigint error");
        exit(1);
    }

    pid_t producent, konsument; 
    char pathProducent[80]; // sciezka do producenta
    char pathKonsument[80]; // sciezka do konsumenta
    sprintf(pathProducent, "%s%s", PATH, argv[1]);
    sprintf(pathKonsument, "%s%s", PATH, argv[2]);

    switch(producent = fork()) // utworzenie producenta
    {
        case -1:
            perror("fork error producent");
            exit(1);
        case 0:
            execlp(pathProducent, argv[1],PROD, KONS, SHAR_MEM, argv[3], NULL); // wywowalnie procesu producent
            perror("excelp error producent");
            _exit(2);
    }

    switch(konsument = fork()) // utworzenie konsumenta
    {
        case -1:
            perror("fork error konsument");
            exit(1);
        case 0:
            execlp(pathKonsument, argv[2],PROD, KONS, SHAR_MEM, argv[4], NULL); // wywowalnie procesu producent
            perror("excelp error konsument");
            _exit(2);
    }

    if (waitpid(producent, NULL, 0) == -1) // czekanie na producenta
    {
        perror("waitpid producent error");
        exit(1);
    }
    if (waitpid(konsument, NULL, 0) == -1) // czekanie na konsumenta
    {
        perror("waitpid konsument error");
        exit(1);
    }

    zamknij_pamiec_dzielona(sm_fd);
    exit(0);
}