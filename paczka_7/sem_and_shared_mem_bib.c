#include "sem_and_shared_mem_bib.h"
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
    sem_t * adr = sem_open(name, O_CREAT | O_EXCL, 0777, value);
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
void usun_semafor(const char *name)
{
    switch(sem_unlink(name))
    {
        case -1:
            perror("sem_unlink error");
            _exit(EXIT_FAILURE);
        case 0:
            printf("Pomyslnie usunieto semafor %s !\n", name);
            break;

    }
    
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
int pobierz_wartosc_semafora(sem_t *sem)
{
    int sval;
    if (sem_getvalue(sem, &sval) == -1)
    {
        perror("sem_getvalue error");
        exit(EXIT_FAILURE);
    }
    return sval;
}
/*******************************************************************/





int stworz_pamiec_dzielona(const char *name)
{
    int fd = shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0644);
    if (fd == -1)
    {
        perror("shm_open shared mem error (create)");
        exit(1);
    }
    return fd;
}
/*******************************************************************/
int otworz_pamiec_dzielona(const char *name)
{
    
    int fd = shm_open(name, O_RDWR, 0644);
    if (fd == -1)
    {
        perror("shm_open shared mem error (open)");
        exit(1);
    }
    return fd;
}

void ustaw_dlugosc_pamieci_dzielone(int fd, off_t length)
{
    if (ftruncate(fd, length) == -1)
    {
        perror("ftruncate error");
        exit(1);
    }

}

/*******************************************************************/

Towar* odzworuj_w_wirtualna_przestrzen_adr(int fd)
{
    Towar *towar = (Towar *) mmap(NULL, sizeof(Towar), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (towar == MAP_FAILED)
    {
        perror("mmap error");
        exit(1);
    }
    return towar;
}
/*******************************************************************/
void usun_odzwzorowanie_wirtualnej_przestrzeni(Towar *addr)
{
    if (munmap(addr, sizeof(Towar)) == -1)
    {
        perror("munmap error");
        exit(1);
    }
}
/*******************************************************************/
void zamknij_pamec_dzielona(int fd)
{
    if (close(fd) == -1)
    {
        perror("close shared memory error");
        exit(1);
    }
}
/*******************************************************************/
void usun_pamiec_dzielona(const char *shm_name)
{
    switch(shm_unlink(shm_name))
    {
        case -1:
            perror("shm unlink error");
            _exit(1);
        case 0:
            printf("Pomyslnie usunieto pamiec dzielona!\n");
            break;
    }
}