/*
========================================================================
Autor: Bartłomiej Kachnic,                           Krakow, 27.04.2022
     
     Proces Konsumenta odczytuje dane z potoku
        i wpisuje je do pliku.
========================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#define BUFOR 12
#define PATH "./"
int main(int argc, char *argv[]) // 1 - potok nazwany, 2 - wyniki.txt
{
    if (argc != 3)
    {
        perror("Wrong number of args (Konsument) Example: ./konsument fifo results.txt");
        _exit(EXIT_FAILURE);
    }
    /*********************************************/
    char buff[BUFOR]; // bufor dla konsumenta
    sleep(2); // czekanie
    srand(time(NULL)); // ustawienia czasu
    unsigned int t; // losowa liczba dla sleep()
    unsigned int n; // losowa liczba ilosc danych
    /*********************************************/
    
    int deskryptor_fifo = open(argv[1], O_RDONLY, 0777); // read fifo
    if (deskryptor_fifo == -1)
    {
        perror("Blad otwarcia pliku danych do czytania!");              // sprawdzenie czy udalo sie otworzyc pliki
        _exit(EXIT_FAILURE);
    }
    
    char PATHFILE_wyniki[86];
    sprintf(PATHFILE_wyniki, "%s%s", PATH, argv[2]);             // wyniki.txt
    int deskryptor_wyniki = open(PATHFILE_wyniki, O_WRONLY, 0777);
    /*********************************************/
    
    if (deskryptor_wyniki == -1)
    {
        perror("Blad otwarcia potoku nazwanego do zapisu(producent)");
        _exit(EXIT_FAILURE);
    }
    /*********************************************/
    
    int ile_bit;  // ile bitow do odczytania z bufora
    
    n = (rand() % 8 + 1); // losowa liczba od 1 do 8
    
    while((ile_bit = read(deskryptor_fifo, buff, n)))
    {
        if (ile_bit == -1)
        {
            perror("read error konsument");
            _exit(1);
        }
        
        t = (rand() % 3 + 1); // losowa liczba od 1 do 3
        n = (rand() % 8 + 1); // losowa liczba od 1 do 8
        sleep(t); // czekanie
        if (write(deskryptor_wyniki, buff, ile_bit) == -1)
        {
            _exit(1);
        }
        
        if (write(STDOUT_FILENO, "Konsument: ", sizeof(char)*12) == -1)
            {
                perror("STDOUT konsument error");
                _exit(1);
            }
        if (write(STDOUT_FILENO, buff,ile_bit) == -1)
            {
                perror("STDOUT konsument error");
                _exit(1);
            } 
        if (write(STDOUT_FILENO, "\n", sizeof(char)*2) == -1)
            {
                perror("STDOUT konsument error");
                _exit(1);
            }

        sleep(t); // czekanie
       
        
        
        
    
    }
    
    if (close(deskryptor_fifo) == -1)  // zamkniecie fifo do czytania
    {
        perror("close fifo error konsument");
        _exit(1);
    }
    if (close(deskryptor_wyniki) == -1)
    {
        perror("close wyniki error konsument"); // zamkniecie pliku do pisania
        _exit(1);
    }
    
    
    return 0;
}