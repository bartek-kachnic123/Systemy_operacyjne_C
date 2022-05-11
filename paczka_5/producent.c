/*
========================================================================
Autor: Bartłomiej Kachnic,                           Krakow, 27.04.2022

       Proces producenta pobiera dane z pliku i umieszcza je w potoku fifo.
      
========================================================================
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#define BUFOR 15
#define PATH "./"

int main(int argc, char *argv[]) // 1 - nazwa potoku nazwanego 2 - dane.txt
{
    if (argc != 3)
    {
        perror("Wrong number of args (Producent), Example: ./producent fifo data.txt");
        _exit(EXIT_FAILURE);
    }
    /*********************************************/
    srand(time(NULL)); // ustawienie czasu 
    unsigned int t; // losowa liczba dla sleep()
    unsigned int n; // losowa liczba ilosc danych
    char buff[BUFOR];  // bufor dla producenta
    /*********************************************/


    /*********************************************/
               // potok nazwany fifo
    int deskryptor_fifo = open(argv[1], O_WRONLY, 0777);
    if (deskryptor_fifo == -1)
    {
        perror("Blad otwarcia potoku nazwanego do zapisu(producent)");
        _exit(EXIT_FAILURE);
    }
    /*********************************************/
                    // dane.txt
    char PATHFILE_dane[86];
    sprintf(PATHFILE_dane, "%s%s", PATH, argv[2]);             
    int deskryptor_dane = open(PATHFILE_dane, O_RDONLY, 0777);
    /*********************************************/
    

    /*********************************************/
    if (deskryptor_dane == -1) // sprawdzenie czy udalo sie otworzyc plikl
    {
        perror("Blad otwarcia pliku danych do czytania!");              
        _exit(EXIT_FAILURE);
    }
    
   
    
    
    int ile_bit; // ile bitow odczytano
    n = (rand() % 8 + 1); // losowa liczba od 1 do 8
    while((ile_bit = read(deskryptor_dane, buff, n)))
    {   
        if (ile_bit == -1)
        {
            perror("error read producent");
            _exit(1);
        }
        
        n = (rand() % 8 + 1); // losowa liczba od 1 do 8
        t = (rand() % 4 + 3); // losowa liczba od 1 do 3
        
        if(write(deskryptor_fifo, buff,ile_bit) == -1)
        {
            perror("write error producent");
            _exit(1);
        }
        
        // Wypisywanie na ekran:
        if (write(STDOUT_FILENO, "Producent: ", 12) == -1)
            {
                perror("STDOUT producent error");
                _exit(1);
            }
        if (write(STDOUT_FILENO, buff,ile_bit) == -1)
            {
                perror("STDOUT producent error");
                _exit(1);
            } 
        if (write(STDOUT_FILENO, "\n", 2) == -1)
            {
                perror("STDOUT producent error");
                _exit(1);
            }
        sleep(t); // czekanie
        
    
        //end while
    }
    
    if (close(deskryptor_fifo) == -1) // zamkniecie potoku do pisania
    {
        perror("close fifo error");
        _exit(1);
    } 
    if (close(deskryptor_dane) == -1) // zamkniecie pliku do czytania
    {
        perror("close dane error");
        _exit(1);
    } 

    

    return 0;
}