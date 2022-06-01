
/*
========================================================================
Autor: Bartłomiej Kachnic,                           Krakow, 01.06.2022

    Program otwiera kolejke servera, po czym prosi o podanie dzialania,
    ktore zostanie wyslane do servera. Po otrzymaniu odpowiedzi wypisuje
    ja na ekranie.
      
========================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "mqueue_biblio.h"
#define NBUF 50

//====================================
mqd_t *ptr_mq_server_des;
mqd_t *ptr_mq_client_des;
char *ptr_client_queue_name;
void exit_handler() // funkcja wykonujaca sie przed zakonczeniem programu
{
    // zamkniecie kolejki serwera
    close_mqueue(*ptr_mq_server_des);
    
    // zamkniecie kolejki klienta
    close_mqueue(*ptr_mq_client_des);

    // usuniecie kolejki klienta
    unlink_mqueue(ptr_client_queue_name);
}
//====================================
void sig_handler(int num) // sygnal sigint
{
    exit(0);
}
//====================================

int main(int argc, char *argv[])
{

    if (atexit(exit_handler) != 0) // rejestracja funkcji atexit
    {
        perror("atexit error");
        exit(2);
    }

    if (signal(SIGINT, sig_handler) == SIG_ERR) // rejestracja funckji obslugujacej syganl SIGINT
    {
        perror("signal sigint error");
        exit(1);
    }

    char bufor[NBUF]; // bufor dla   dzialania
    char client_queue_name[MQ_MSGSIZE-NBUF]; // nazwa kolejki klienta
    char msg[MQ_MSGSIZE]; // bufor dla wiadomosci gotowej do wyslania

    // sklejanie nazwy kolejki / + pid
    sprintf(client_queue_name,"/%d", getpid());
    ptr_client_queue_name = client_queue_name;
    
    struct mq_attr attr; // struktura atrybutow kolejki
    attr.mq_flags =  0;
    attr.mq_maxmsg = MQ_MAXMSG; // ustawienie max ilosci komunikatow
    attr.mq_msgsize = MQ_MSGSIZE; // ustawienie dlugosci komunikatu
    attr.mq_curmsgs = 0;

    mqd_t mq_client_des; // deskryptor kolejki klienta
    ptr_mq_client_des = &mq_client_des;
    mq_client_des = create_mqueue(client_queue_name, &attr); // stworzenie kolejki

    // wypisanie wlasciwosci kolejki:
    printf("Nazwa kolejki: %s, deskryptor: %d oraz atrybuty:\n", client_queue_name, mq_client_des);
    printf("flags: %ld, maxmsg: %ld, maxsize: %ld, curmsgs: %ld!\n", attr.mq_flags, attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);

    sleep(1); // czekanie na serwer
    
    
    mqd_t mq_server_des; // deskryptor kolejki serwera
    ptr_mq_server_des = &mq_server_des;

    // otwarcie kolejki serwera
    mq_server_des = open_mqueue(MQUEUE_SERVER);

    printf("Wyslij zapytanie do serwera z dzialaniem np 2+3. Dostepne operatory(+,-,*,/)!\n");
    while (1)
    {
        printf("Podaj dzialanie: ");
        // pobranie wiadomosci
        if (fgets(bufor, NBUF, stdin) == NULL) break; 
        
        // sklejanie wiadomosci pid + msg
        sprintf(msg, "%s %s", client_queue_name, bufor);

        // wyslanie wiadomosci
        send_msg(mq_server_des, msg, MQ_MSGSIZE, 1);

        // odbieranie wiadomosci z serwera
        receive_msg(mq_client_des, msg, MQ_MSGSIZE);
        printf("Message from server : %s \n", msg);

    }
    printf("Koniec!\n");

    exit(0);
}