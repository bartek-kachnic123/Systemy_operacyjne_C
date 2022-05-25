#include "shm_biblio.h"

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

Towar* odzwzoruj_w_wirtualna_przestrzen_adr(int fd)
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
void zamknij_pamiec_dzielona(int fd)
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