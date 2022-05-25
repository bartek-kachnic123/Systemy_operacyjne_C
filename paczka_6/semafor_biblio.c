#include "semafor_biblio.h"
/***************************************************************************/
void utworz_semafor_nienazwany(sem_t *sem, int pshared, unsigned int value)
{
    if (sem_init(sem, pshared, value) == -1)
    {
        perror("sem init error");
        _exit(EXIT_FAILURE);
    }
}
/*******************************************************************/
void usun_semafor_nienazwany(sem_t *sem)
{
    if (sem_destroy(sem) == -1)
    {
        perror("sem_destroy error");
        _exit(EXIT_FAILURE);
    }
}
/*******************************************************************/
sem_t *utworz_semafor_nazwany(const char *name, int value)
{
    sem_t * adr = sem_open(name, O_CREAT | O_EXCL, 0644, value);
    if (adr == SEM_FAILED)
    {
        perror("sem_open error");
        exit(EXIT_FAILURE);
    }
    return adr;
}
/*******************************************************************/
sem_t *otworz_semafor_nazwany(const char *name)
{
    sem_t * adr = sem_open(name ,O_RDWR);
    if (adr == SEM_FAILED)
    {
        perror("sem_open error");
        exit(EXIT_FAILURE);
    }
    return adr;
}
/*******************************************************************/
void zwolnij_zasoby_semafora(sem_t *sem)
{
    if (sem_close(sem) == -1)
    {
        perror("sem_close error");
        exit(EXIT_FAILURE);
    }
}

/*******************************************************************/

void usun_semafor(void)
{
    if (sem_unlink(name_semaphore) == -1)
    {
        perror("sem_unlink error");
        _exit(EXIT_FAILURE);
    }
    printf("Pomyslnie zamknieto semafor!\n");
}

/*******************************************************************/
void V_sem_post(sem_t *sem)
{
    if (sem_post(sem) == -1)
    {
        perror("sem_post error");
        exit(EXIT_FAILURE);
    }
}
/*******************************************************************/
void P_sem_wait(sem_t *sem)
{
    if (sem_wait(sem) == -1)
    {
        perror("sem_wait error");
        exit(EXIT_FAILURE);
    }
}
/*******************************************************************/
void pobierz_wartosc_semafora(sem_t *sem, int *sval)
{
    if (sem_getvalue(sem, sval) == -1)
    {
        perror("sem_getvalue error");
        exit(EXIT_FAILURE);
    }
}
/*******************************************************************/