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

char pathname[] = "09-1a.c"; /* Имя файла, использующееся для генерации ключа.
Файл с таким именем должен существовать в текущей директории */

key_t key; /* IPC ключ */

int len, maxlen; /* Реальная длина и максимальная длина информативной части сообщения */

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

//отправка

mybuf.mtype=1;
mybuf.info.PID=getpid();
mybuf.info.num=2;
len=sizeof(mybuf.info);

if(msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0)<0){
printf("Can\'t send message to queue\n");
msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
exit(-1);
}


maxlen = sizeof(mybuf.info);

if(( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, getpid(), 0)) < 0){
printf("Can\'t receive message from queue\n");
exit(-1);
}

printf("message type = %ld, square =%d\n", mybuf.mtype,mybuf.info.num);
printf("\n");




return 0; /* Исключительно для отсутствия warning'ов при компиляции. */
}