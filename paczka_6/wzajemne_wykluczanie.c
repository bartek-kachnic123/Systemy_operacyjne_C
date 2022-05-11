/*
========================================================================
Autor: Bartłomiej Kachnic,                           Krakow, 11.05.2022

    Przy pomocy semaforow nazwanych POSIX program realizuje wzajmne wykluczanie procesow.
    Program wypisuje odpowiednie informacje przed, w sesji krytycznej i po. W sesji procesy
    odwoluja sie do wspolnych danych.
      
========================================================================
*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include "semafor_biblio.h"
#define PATH "./"

int main(int argc, char *argv[]) // argv[1] - nazwa semafora, argv[2] - file.txt, argv[3] - numer sekcji krytycznej
{
    if (argc != 4)
    {
        perror("wrong number of args! Example: ../programm semaphore_name file.txt num_of_cricital_section");
        _exit(EXIT_FAILURE);
    }
    FILE *fin, *fout; // wskazniki do funkcji fopen
    int value; // wartosc z pliku
    int sval; // wartosc semafora
    char pathNameFile[86]; // sciezka do pliku txt
    srand(time(NULL)); // ustawienie czasu
    unsigned int t = rand() % 3 + 1; // czas od 1 do 3 sekund
    sprintf(pathNameFile, "%s%s", PATH, argv[2]);
    sem_t *sem = otworz_semafor_nazwany(argv[1]); // adres semafora

    printf("Pid procesu: %u ", getpid()); // wypisanie pid procesu
    pobierz_wartosc_semafora(sem, &sval); // pobranie wartosci semafora
    printf("PRZED SEKCJA KRYTYCZNA: %d ", sval);
    
    sleep(t); // czekanie
    P_sem_wait(sem); 
    sleep(t); // czekanie

   /**************************************************************************/
    // sekcja krytyczna:

    pobierz_wartosc_semafora(sem, &sval); // pobranie wartosci semafora
    printf("\tSEKCJA KRYTYCZNA %s: %d ", argv[3], sval);
    fin = fopen(pathNameFile, "r");
    if (!fin)
    {
        perror("cant read file");
        _exit(EXIT_FAILURE);
    }
    if (fscanf(fin, "%d", &value) < 0)  // odczytanie wartosci z pliku
    {
        perror("fscanf error with file");
        _exit(EXIT_FAILURE);
    }
    fclose(fin); // zamkniecie pliku do odczytu
    printf("Numer z pliku : %d!", value); // wypisanie numeru na ekran
    value++; // zwiekszenie o 1 wartosci;
    sleep(t); // czekanie

    fout = fopen(pathNameFile, "w");
    if (!fout)
    {
        perror("cant write to file");
        _exit(EXIT_FAILURE);
    }
    if (fprintf(fout, "%d", value) < 0) // wpisanie wartosci do pliku
    {
        perror("fprintf error with file");
        _exit(EXIT_FAILURE);
    }
    fclose(fout); // zamkniecie pliku do zapisu

    // koniec sekcji krytycznej
    /**************************************************************************/

    V_sem_post(sem);
    pobierz_wartosc_semafora(sem, &sval); // pobranie wartosci semafora
    printf("\tPO SEKCJA KRYTYCZNNEJ : %d \n", sval);









    zwolnij_zasoby_semafora(sem); // zwolnienie zasobow
    return 0;
}