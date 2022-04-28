/*
========================================================================
Autor: Bartłomiej Kachnic,                           Krakow, 18.04.2022

Program realizuje problem Producenta-Konsumenta za pomoca potokow nienazwanych
       Proces producenta pobiera dane z pliku i umieszcza je w potoku,
       natomiast proces konsumenta(macierzysyty) odczytuje dane z potoku
       i wpisuje je do pliku.
========================================================================
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#define BUFOR 10



int main(int argc, char *argv[]) // 1 - nazwa pliku do czytania, 2 - nazwa pliku do pisania
{
    if (argc!=3)
    {
        perror("Wrong number of args (MAIN) Example:  ./prog_1 data.txt results.txt");
        exit(EXIT_FAILURE);
    }

    int fd[2]; // fd[0] - read, fd[1] - write
    if (pipe(fd) == -1) // tworzenie potoku
    {
      perror("pipe error main");
      exit(EXIT_FAILURE);
    }
    
    unsigned int t; // losowa liczba dla sleep()
    unsigned int n; // losowa liczba ilosc danych
    char buff[BUFOR];  // bufor 
    int deskryptor; // deskryptor dla plikow dane.txt i wyniki.txt
    int ile_bit; // ile bitow do wyslania
    pid_t producent; // pid producenta

    switch(producent = fork()) // tworzenie procesu producenta
    {

    case -1:
        perror("fork error");
        exit(EXIT_FAILURE);

    case 0: // proces potomny (producent)
        if (close(fd[0]) == -1) // zamkniecie pipe odczytu
        {
            perror("close fd read error (producent)");
        } 
        srand(time(NULL)); // ustawienie czasu dla producenta

        deskryptor = open(argv[1], O_RDONLY); // otwarcie pliku do czytania
        if (deskryptor == -1)
        {
            perror("Blad otwarcia pliku do pisania");
            exit(EXIT_FAILURE);
        }
        
        n = (rand() % 8 + 1); // losowa liczba od 1 do 8
        while((ile_bit = read(deskryptor, buff, n))) // czytanie z pliku
        {   
            if (ile_bit == -1)
            {
                perror("error read producent");
                exit(1);
            }
            
            t = (rand() % 2 + 1); // losowa liczba od 1 do 2
            n = (rand() % 8 + 1); // losowa liczba od 1 do 8
            
            
            
            if(write(fd[1], buff,ile_bit) == -1)  // pisanie do potoku
            {
                perror("write error pipe ");
                exit(1);
            }
            
            // Wypisywanie na ekran:
            if (write(STDOUT_FILENO, "Producent: ", 12) == -1)
            {
                perror("STDOUT producent error");
                exit(1);
            }
            if (write(STDOUT_FILENO, buff,ile_bit) == -1)
            {
                perror("STDOUT producent error");
                exit(1);
            } 
            if (write(STDOUT_FILENO, "\n", 2) == -1)
            {
                perror("STDOUT producent error");
                exit(1);
            }
            sleep(t); // czekanie
        } // end while
        
        if (close(fd[1]) == -1) // zamkniecie potoku do pisania
        {
            perror("close fd write error (producent)");
            exit(1);
        } 
        if (close(deskryptor) == -1) // zamkniecie pliku do czytania
        {
            perror("close error read file (producent)");
            exit(1);
        } 
    

    break;
                
    default:  // proces macierzysty ( konsument)]
        if(close(fd[1]) == -1) // zamkniecie potoku do zapisywania
        {
            perror("close fd write error konsument");
            exit(1);
        } 

        sleep(2); // czekanie
        srand(time(NULL)); // ustawienie czasu dla konsumenta

        deskryptor = open(argv[2], O_WRONLY); // otwarcie pliku do pisania
    
        if (deskryptor == -1)
        {
            perror("Blad otwarcia pliku do pisania");
            exit(EXIT_FAILURE);
        }

        
        n = (rand() % 8 + 1); // losowa liczba od 1 do 8
        
        while((ile_bit = read(fd[0], buff, n)))  // czytanie  z  potoku
        {
            if (ile_bit == -1)
            {
                perror("read error pipe konsument");
                exit(1);
            }
            
            t = (rand() % 3 + 1); // losowa liczba od 1 do 3
            n = (rand() % 8 + 1); // losowa liczba od 1 do 8
            

            if (write(deskryptor, buff, ile_bit) == -1)  // pisanie do pliku
            {
                perror("problem z wpisaniem do pliku");
                exit(1);
            }

            // Wypisywanie na ekran:
            if(write(STDOUT_FILENO, "Konsument: ", sizeof(char)*12) == -1)
            {
                perror("STDOUT konsument error");
                exit(1);
            }
            if (write(STDOUT_FILENO, buff,ile_bit) == -1)
            {
                perror("STDOUT konsument error");
                exit(1);
            } 
            if (write(STDOUT_FILENO, "\n", sizeof(char)*2) == -1)
            {
                perror("STDOUT konsument error");
                exit(1);
            }
            sleep(t); // czekanie
            
        } //end while
        
        if (close(fd[0]) == -1) // zamkniecie potoku do czytani
        {
            perror("close fd read error (konsument");
            exit(1);
        } 
        if (close(deskryptor) == -1) // zamkniecie pliku do pisania
        {
            perror("close write file error (konsument)");
            exit(1);
        } 
        

        if (waitpid(producent, NULL, 0) == -1) // czekanie na producenta
        {
            perror("producent process error");
            exit(EXIT_FAILURE);
        }


        break;
    }
      

    return 0;
}