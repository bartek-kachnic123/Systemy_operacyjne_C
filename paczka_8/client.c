#include <stdio.h>
#include <stdlib.h>
#include "mqueue_biblio.h"
#define NBUF 20


int main(int argc, char *argv[])
{
    char bufor[NBUF]; // bufor dla komunikatu z dzialaniem
    char msg[MQ_MSGSIZE]; // bufor dla wiadomosci gotowej do wyslania
    
    mqd_t mq_server_des;
    printf("Wyslij zapytanie do serwera z dzialaniem np 2+3. Dostepne operatory(+,-,*,/)!\n");
    while (1)
    {
        printf("Podaj dzialanie: ");
        // pobranie wiadomosci
        if (fgets(bufor, NBUF, stdin) == NULL) break; 

        // otwarcie kolejki
        mq_server_des = open_mqueue(MQUEUE_SERVER);
        
        // sklejanie wiadomosci pid + msg
        sprintf(msg, "/%d %s", getpid(), bufor);

        // wyslanie wiadomosci
        send_msg(mq_server_des, msg, MQ_MSGSIZE, 0);


        // fflush(stdout);
        // printf("%s", bufor);

        // zamkniecie kolejki
        close_mqueue(mq_server_des);


    }
    printf("Koniec!\n");
    
    




    return 0;
}