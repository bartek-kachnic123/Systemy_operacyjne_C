#include <stdio.h>
#include <stdlib.h>
#include "sem_and_shared_mem_bib.h"
#define SHAR_MEM "/shared_memory"
#define SHAR_N 20
#define PROD "prod_1"
#define KONS "kons_1"
#define N 20
#define PATH "./"



int main(int argc, char *argv[]) // 1-producent, 2-konsument, 3-dane, 4- wyniki
{
    if (argc != 5)
    {
        printf("Wrong numer of args! Example: ./%s producent.x konsument.x data.txt results.txt", argv[0]);
        _exit(1);
    }
    
    sem_t *prod = utworz_semafor_nazwany(PROD, N); // semafor do wstrzymania producenta
    sem_t *kons = utworz_semafor_nazwany(KONS, 0); // semafor do wstrzymania konsumenta
    
    
    printf("Semafor %s o adresie %p i wartosci %d!\n", PROD, (void *) prod, pobierz_wartosc_semafora(prod));
    printf("Semafor %s o adresie %p i wartosci %d!\n", KONS, (void *) kons, pobierz_wartosc_semafora(kons));
    int sm_fd = stworz_pamiec_dzielona(SHAR_MEM); // stworzenie pamieci dzielonej
    ustaw_dlugosc_pamieci_dzielone(sm_fd, SHAR_N);
    
    printf("Pamiec dzielona o deskryptorze %d i rozmiarze %d!\n", sm_fd, SHAR_N);
    Towar *bufor = odzworuj_w_wirtualna_przestrzen_adr(sm_fd);
    bufor->wstaw = bufor->wyjmij = 0;

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
    if (waitpid(konsument, NULL, 0) == -1)
    {
        perror("waitpid konsument error");
        exit(1);
    }

    
    usun_semafor(PROD);
    usun_semafor(KONS);
    
    zamknij_pamec_dzielona(sm_fd);
    usun_pamiec_dzielona(SHAR_MEM);















    return 0;
}