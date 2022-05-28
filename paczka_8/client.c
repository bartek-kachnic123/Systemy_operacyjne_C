#include <stdio.h>
#include <stdlib.h>
#include "mqueue_biblio.h"
#define NBUF 20


int main(int argc, char *argv[])
{
    char bufor[NBUF]; // bufor dla komunikatu z dzialaniem
    char client_queue_name[NBUF]; // nazwa kolejki klienta
    char msg[MQ_MSGSIZE]; // bufor dla wiadomosci gotowej do wyslania
    // sklejanie nazwy kolejki / + pid
    sprintf(client_queue_name,"/%d", getpid());
    
    struct mq_attr attr; // struktura atrybutow kolejki
    attr.mq_flags =  0;
    attr.mq_maxmsg = MQ_MAXMSG; // ustawienie max ilosci komunikatow
    attr.mq_msgsize = MQ_MSGSIZE; // ustawienie dlugosci komunikatu
    attr.mq_curmsgs = 0;
    mqd_t mq_client_des; // deskryptor kolejki klienta
    mq_client_des = create_mqueue(client_queue_name, &attr); // stworzenie kolejki

    // wypisanie wlasciwosci kolejki:
    printf("Nazwa kolejki: %s, deskryptor: %d oraz atrybuty:\n", client_queue_name, mq_client_des);
    printf("flags: %ld, maxmsg: %ld, maxsize: %ld, curmsgs: %ld!\n", attr.mq_flags, attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);

    mqd_t mq_server_des; // deskryptor kolejki serwera

    printf("Wyslij zapytanie do serwera z dzialaniem np 2+3. Dostepne operatory(+,-,*,/)!\n");
    while (1)
    {
        printf("Podaj dzialanie: ");
        // pobranie wiadomosci
        if (fgets(bufor, NBUF, stdin) == NULL) break; 
        fflush(stdout);
        // otwarcie kolejki
        mq_server_des = open_mqueue(MQUEUE_SERVER);
        
        

        // sklejanie wiadomosci pid + msg
        sprintf(msg, "%s %s", client_queue_name, bufor);

        // wyslanie wiadomosci
        send_msg(mq_server_des, msg, MQ_MSGSIZE, 0);


        // fflush(stdout);
        // printf("%s", bufor);

        // zamkniecie kolejki serwera
        close_mqueue(mq_server_des);


        // odbieranie wiadomosci z serwera


    }
    printf("Koniec!\n");
    
    // zamkniecie kolejki klienta
    close_mqueue(mq_client_des);

    // usuniecie kolejki klienta
    unlink_mqueue(client_queue_name);
    




    return 0;
}