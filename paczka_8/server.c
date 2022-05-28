#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "mqueue_biblio.h"

void sig_handler(int num)
{
    exit(0);
}

void exit_handler(void) // funkcja wykonujaca sie przed zakonczeniem programu
{
    unlink_mqueue(MQUEUE_SERVER); // usun kolejke
}

int main()
{
    struct mq_attr attr; // struktura atrybutow kolejki
    attr.mq_flags =  0;
    attr.mq_maxmsg = MQ_MAXMSG; // ustawienie max ilosci komunikatow
    attr.mq_msgsize = MQ_MSGSIZE; // ustawienie dlugosci komunikatu
    attr.mq_curmsgs = 0;

    if (signal(SIGINT, sig_handler) == SIG_ERR) // obsluga sygnalu SIGINT
    {
        perror("signal sigint error");
        exit(EXIT_FAILURE);

    }
    if (atexit(exit_handler) != 0) // rejestracja funkcji atexit
    {
        perror("atexit function errror");
        exit(EXIT_FAILURE);
    }

    mqd_t mqdes; // deskryptor kolejki
    mqdes = create_mqueue(MQUEUE_SERVER, &attr); // utworzenie kolejki

    // wypisanie wlasciwosci kolejki:
    printf("Nazwa kolejki: %s, deskryptor: %d oraz atrybuty:\n", MQUEUE_SERVER, mqdes);
    printf("flags: %ld, maxmsg: %ld, maxsize: %ld, curmsgs: %ld!\n", attr.mq_flags, attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);

    char msg[MQ_MSGSIZE]; // wiadomosc od klienta
    while(1)
    {
        // odbieranie wiadomosci
        receive_msg(mqdes, msg, MQ_MSGSIZE);
        
        fflush(stdout);
        printf("Message from client: %s", msg);
        
    } // petla nieskonczona


    close_mqueue(mqdes); // zamknij kolejke
    




    return 0;
}