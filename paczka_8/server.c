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

    char msg[MQ_MSGSIZE]; // wiadomosc 
    mqd_t mq_client_desc; // deskryptor kolejki klienta
    char client_pid[20];
    int num1, num2; 
    char operator;
    int result;
    int is_operator = 1; // 1 - operator jest w zbiorze {+,-,*,/}
    while(1)
    {
        // odbieranie wiadomosci
        receive_msg(mqdes, msg, MQ_MSGSIZE);
        // wypisanie wiadomosci
        
        printf("From client: %s", msg);

        // wyciaganie danych z wiadomosci
        if (sscanf(msg,"%s %d%c%d", client_pid, &num1, &operator, &num2) != 4)
        {
            perror("sscanf error");
            exit(1);
        }

        // obliczanie wyniku
        if (operator == '/' && num2 == 0) // dzielenie przez 0;
        {
            sprintf(msg, "Nie wolno dzielic przez 0!");
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
                sprintf(msg, "Result: %d", result);
                
            }
            else 
            {
                sprintf(msg, "Operator nie znajduje sie w zbiorze dostepnych operacji {+,-,*,/");
            }
        } // end if
        // wypisanie wyniku
        printf("%s", msg);
    
       // otworzenie kolejki klienta
       mq_client_desc = open_mqueue(client_pid);


       // wyslanie odpowiedzi do klienta
       send_msg(mq_client_desc, msg, MQ_MSGSIZE, 0);
       sleep(1);
       


       // zamkniecie kolejki klienta
       close_mqueue(mq_client_desc);

        
        
    
    } // end while 


    close_mqueue(mqdes); // zamknij kolejke
    




    return 0;
}