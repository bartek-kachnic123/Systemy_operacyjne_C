#include <stdio.h>
#include <stdlib.h>
#include "sem_and_shared_mem_bib.h"
#define N  20

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
   


    printf("Konsument: %s %p, %s %p, Deskryptor pamieci dzielonej: %d!\n", argv[1], (void*) prod, argv[2], (void *) kons, sm_fd);



    zwolnij_zasoby_semafora(prod);
    zwolnij_zasoby_semafora(kons);
    zamknij_pamec_dzielona(sm_fd);

    


    return 0;
}