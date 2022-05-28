#include <stdio.h>
#include <stdlib.h>
#include "mqueue_biblio.h"



int main(int argc, char *argv[])
{
    char bufor[MQ_MAXMSG]; // bufor dla komunikatu z dzialaniem
    printf("Wyslij zapytanie do serwera z dzialaniem np 2+3. Dostepne operatory(+,-,*,/)!\n");
    while (1)
    {
        printf("Podaj dzialanie: ");
        if (fgets(bufor, MQ_MAXMSG - 1, stdin) == NULL) break; // pobranie wiadomosci od uzytkownika
        fflush(stdout);
        printf("%s", bufor);
    }
    printf("Koniec!\n");
    
    




    return 0;
}