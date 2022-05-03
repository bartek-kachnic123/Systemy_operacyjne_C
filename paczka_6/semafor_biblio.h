#if !defined(SEMAFOR_BIBLIO)
#define SEMAFOR_BIBLIO
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <semaphore.h>

void utworz_semafor_nienazwany(sem_t *sem, int pshared, unsigned int value); 
void usun_semafor_nienazwany(sem_t *sem);
sem_t *utworz_semafor_nazwany(const char *name, int value);
sem_t *otworz_semafor_nazwany(const char *name);
void zwolnij_zasoby_semafora(sem_t *sem);
char *name_semaphore;
void usun_semafor(void);
void V_sem_post(sem_t *sem);
void P_sem_wait(sem_t *sem);
void pobierz_wartosc_semafora(sem_t *sem, int *sval);

#endif // SEMAFOR_BIBLIO
