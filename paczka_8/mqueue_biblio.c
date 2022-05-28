#include "mqueue_biblio.h"

mqd_t create_mqueue(const char *name, struct mq_attr *attr)
{
    mqd_t mqueue_descriptor = mq_open(name, O_RDONLY | O_CREAT | O_EXCL, 0644, attr);
    if (mqueue_descriptor == (mqd_t)-1)
    {
        perror("create queue error");
        exit(EXIT_FAILURE);
    }
    return mqueue_descriptor;
}

//========================================================================

mqd_t open_mqueue(const char *name)
{
    mqd_t mqueue_descriptor = mq_open(name, O_WRONLY);
    if (mqueue_descriptor == (mqd_t)-1)
    {
        perror("create queue error");
        exit(EXIT_FAILURE);
    }
    return mqueue_descriptor;
}

//========================================================================
void close_mqueue(mqd_t mqdes)
{
    if (mq_close(mqdes) == -1)
    {
        perror("mq_close error");
        exit(EXIT_FAILURE);
    }
}
//========================================================================
void unlink_mqueue(const char *name)
{
    switch (mq_unlink(name))
    {
        case -1:
        perror("mq_unlink error");
        _exit(EXIT_FAILURE);

        case 0:
            printf("Pomyslnie usunieto kolejke o nazwie %s!\n", name);
    }
}

//========================================================================
struct mq_attr getattr_mqueue(mqd_t mqdes)
{
    struct mq_attr attr;
    if (mq_getattr(mqdes, &attr) == -1)
    {
        perror("mq_getattr error");
        exit(EXIT_FAILURE);
    }
    return attr;
}
//========================================================================
void setattr_mqueue(mqd_t mqdes, long mq_flags)
{
    struct mq_attr new_attr;
    struct mq_attr old_attr = getattr_mqueue(mqdes);
    new_attr.mq_flags = mq_flags;

    if (mq_setattr(mqdes, &new_attr, &old_attr) == -1)
    {
        perror("mq_setattr error");
        exit(EXIT_FAILURE);
    }
}
//========================================================================

void send_msg(mqd_t mqdes, const char *msg, size_t msg_len, int msg_prio)
{
    if (mq_send(mqdes, msg, msg_len, msg_prio) == -1)
    {
        perror("mq_send error");
        exit(EXIT_FAILURE);
    }
}

//========================================================================
void receive_msg(mqd_t mqdes, char *msg, size_t msg_len)
{
    if (mq_receive(mqdes, msg, msg_len, NULL) == -1)
    {
        perror("mq_receive erorr");
        exit(EXIT_FAILURE);
    }
}
