#include <stdio.h>
#include <stdlib.h>
#include "mqueue_biblio.h"


int main()
{
    struct mq_attr attr; // struktura atrybutow kolejki
    attr.mq_flags =  0;
    attr.mq_maxmsg = MQ_MAXMSG; // ustawienie max ilosci komunikatow
    attr.mq_msgsize = MQ_MSGSIZE; // ustawienie dlugosci komunikatu
    attr.mq_curmsgs = 0;

    mqd_t mqdes; // deskryptor kolejki
    mqdes = create_mqueue(MQUEUE_SERVER, &attr); // utworzenie kolejki

    
    printf("Nazwa kolejki: %s, deskryptor: %d oraz atrybuty:\n", MQUEUE_SERVER, mqdes);
    printf("flags: %ld, maxmsg: %ld, maxsize: %ld, curmsgs: %ld!\n", attr.mq_flags, attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);




    close_mqueue(mqdes); // zamknij kolejke
    unlink_mqueue(MQUEUE_SERVER); // usun kolejke




    return 0;
}