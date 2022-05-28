#ifndef MQUEUE_BIBLIO
#define MQUEUE_BIBLIO

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h>
#define MQUEUE_SERVER "/queue_server"
#define MQ_MAXMSG 10
#define MQ_MSGSIZE 80



// funkcja tworzy kolejke do odbierania komunikatow za pomoca nazwy i struktury atrybutow.Zwraca deskryptor do niej.
mqd_t create_mqueue(const char *name, struct mq_attr *attr);

// funkcja otwiera kolejke do wysylania komunikatow za pomoca nazwy i zwraca deskryptor do niej.
mqd_t open_mqueue(const char *name);

// funkcja zamykajaca kolejke o deskryptorze mqdes.
void close_mqueue(mqd_t mqdes);

// funkcja usuwajaca kolejke o nazwie name.
void unlink_mqueue(const char *name);

// funkcja pobierajaca atrybuty z kolejki o deskryptorze mqdes
struct mq_attr getattr_mqueue(mqd_t mqdes);

// funkcja ustawiajaca atrybut mq_flags
void setattr_mqueue(mqd_t mqdes, long mq_flags);

// funkcja wysyla wiadomosc msg o dlugosci msg_len i priotytecie msg_prio do kolejki o deskryptorze mqdes
void send_msg(mqd_t mqdes, const char *msg, size_t msg_len, int msg_prio);

// funkcja zapisuje wiadomosc z kolejki o deskryptorze mqdes do bufora msg o dlugosci msg_len
void receive_msg(mqd_t mqdes, char *msg, size_t msg_len);
  

#endif