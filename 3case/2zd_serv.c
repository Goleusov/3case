/*Напишите, откомпилируйте и прогоните программы сервера и клиентов для предложенной
 схемы мультиплексирования сообщений.*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main()
{

int msqid; /* IPC дескриптор для очереди сообщений */

char pathname[] = "09-1a.c"; 

key_t key; /* IPC ключ */

int len, maxlen; /* Cчетчик цикла и длина информативной части сообщения */

struct mymsgbuf
{
long mtype;
struct{
int PID;
int num;
}info;
} mybuf;

if((key = ftok(pathname,0)) < 0){
printf("Can\'t generate key\n");
exit(-1);
}

if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
printf("Can\'t get msqid\n");
exit(-1);
}

// получение
while(1)
{
maxlen = sizeof(mybuf.info);

if(( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 1, 0)) < 0){
printf("Can\'t receive message from queue\n");
exit(-1);
}

//передача

mybuf.mtype = mybuf.info.PID;
mybuf.info.num= mybuf.info.num*mybuf.info.num;
len = sizeof(mybuf.info);

if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
printf("Can\'t send message to queue\n");
msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
exit(-1);
}
}

return 0;
}