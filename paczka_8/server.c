/*
========================================================================
Autor: Bartłomiej Kachnic,                           Krakow, 01.06.2022

    Program tworzy wlasna kolejke do odbierania komunikatow od klientow.
    Po odebraniu komunikatu otwiera kolejke klienta oznaczona jego nr PID
    i wysyla odpowiedz.
      
========================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "mqueue_biblio.h"

//====================================
void sig_handler(int num) // sygnal sigint
{
    exit(0);
}
//====================================
mqd_t *ptr_mqdes; // wskaznik do adresu deskryptora kolejki servera
void exit_handler(void) // funkcja wykonujaca sie przed zakonczeniem programu
{
    close_mqueue(*ptr_mqdes); // zamknij kolejke servera
    unlink_mqueue(MQUEUE_SERVER); // usun kolejke
}
//====================================

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
    ptr_mqdes = &mqdes;

    // wypisanie wlasciwosci kolejki:
    printf("Nazwa kolejki: %s, deskryptor: %d oraz atrybuty:\n", MQUEUE_SERVER, mqdes);
    printf("flags: %ld, maxmsg: %ld, maxsize: %ld, curmsgs: %ld!\n", attr.mq_flags, attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);

    srand(time(NULL)); // pobranie czasu
    int t; // czas w s

    char msg[MQ_MSGSIZE]; // wiadomosc 
    mqd_t mq_client_desc; // deskryptor kolejki klienta
    char client_pid[20]; // pid klienta
    int num1, num2;  // liczba pobrana z komunikatu
    char operator; // {+,-,*,/}
    int result; // wynik dzialania
    int is_operator = 1; // 1 - operator jest w zbiorze {+,-,*,/}
    while(1)
    {
        // odbieranie wiadomosci
        receive_msg(mqdes, msg, MQ_MSGSIZE);

        // wypisanie wiadomosci
        printf("From client: %s", msg);
        fflush(stdout);

        // wyciaganie danych z wiadomosci
        if (sscanf(msg,"%s %d %c %d", client_pid, &num1, &operator, &num2) != 4)
        {
            sprintf(msg, "Zle wprowadzone dzialanie!\n");
        }
        else 
        {
        // obliczanie wyniku
        if (operator == '/' && num2 == 0) // dzielenie przez 0;
        {
            sprintf(msg, "Nie wolno dzielic przez 0!\n");
        }
        else
        {
           switch (operator)
            {
                case '+':
                    result = num1 + num2;
                    break;
                case '-':
                    result = num1 - num2;
                    break;
                case '*':
                    result = num1 * num2;
                    break;
                case '/':
                    result = num1 / num2;
                    break;
                default:
                    is_operator = 0;
                    break;
            }

            if (is_operator)
            {
                sprintf(msg, "Result: %d\n", result);
                
            }
            else 
            {
                is_operator = 1; // resest ustawienia
                sprintf(msg, "Operator nie znajduje sie w zbiorze dostepnych operacji {+,-,*,/}\n");
            }
        }
        } // end if
        // wypisanie wyniku
        printf("Server: %s", msg);
        t = rand() % 3 + 1; // czas od 1 do 3 sekund
        sleep(t);
       // otworzenie kolejki klienta
       mq_client_desc = open_mqueue(client_pid);
       // wyslanie odpowiedzi do klienta
       send_msg(mq_client_desc, msg, MQ_MSGSIZE, 1);
       
       // zamkniecie kolejki klienta
       close_mqueue(mq_client_desc);

    } // end while 

    
    exit(0);
}