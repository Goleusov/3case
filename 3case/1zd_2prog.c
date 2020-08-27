/* Написание, компиляция и прогон программы с организацией взаимоисключения с
помощью семафоров для 2-х процессов, взаимодействующих через разделяемую память. */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <sys/sem.h>
void exit();

int main()
{

int *array; /* Указатель на разделяемую память */

int shmid; /* IPC дескриптор для области разделяемой памяти */
int semid; /* IPC дескриптор для массива IPC семафоров */

int new = 1; /* Флаг необходимости инициализации элементов массива */

char pathname_shm[] = "08-1a.c"; /* Имя файла, использующееся для генерации ключа. Файл с таким именем должен существовать в текущей директории */
char pathname_sem[] = "08-1b.c"; /* Имя файла, использующееся для генерации ключа. Файл с таким именем должен существовать в текущей директории */

key_t key_shm; /* IPC ключ */
key_t key_sem; /* IPC ключ */

struct sembuf mybuf; /* Структура для задания операции над семафором */
long i;

/* Генерируем IPC ключ из имени файла 06-1a.c в текущей директории и номера экземпляра области разделяемой памяти 0 */

if((key_shm = ftok(pathname_shm,0)) < 0){
printf("Can\'t generate key\n");
exit(-1);
}
if((key_sem = ftok(pathname_sem,1)) < 0){
printf("Can\'t generate key\n");
exit(-1);
}

/* Пытаемся эксклюзивно создать разделяемую память для сгенерированного ключа, т.е. если для этого ключа она уже существует, то системный вызов вернет отрицательное значение. Размер памяти определяем как размер массива из 3-х целых переменных, права доступа 0666 - чтение и запись разрешены для всех */

if((semid = semget(key_sem, 1, 0666 | IPC_CREAT)) < 0){
printf("Can\'t get semid\n");
exit(-1);
}

mybuf.sem_op = 1;
mybuf.sem_flg = 0;
mybuf.sem_num = 0;


if(semop(semid, &mybuf, 1) < 0){
printf("Can\'t wait for condition\n");
exit(-1);
}

if((shmid = shmget(key_shm, 3*sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0){

if(errno != EEXIST){

printf("Can\'t create shared memory\n");
exit(-1);
} else {

if((shmid = shmget(key_shm, 3*sizeof(int), 0)) < 0){
printf("Can\'t find shared memory\n");
exit(-1);
}
new = 0;
}
}

if((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)){
printf("Can't attach shared memory\n");
exit(-1);
}
mybuf.sem_op = -1;
if(semop(semid, &mybuf, 1) < 0){
printf("Can\'t wait for condition\n");
exit(-1);
}

if(new){
array[0] = 0;
array[1] = 1;
array[2] = 1;
} else {
array[0] += 1;

for(i=0; i<100000L; i++);

array[2] += 1;
}
mybuf.sem_op = 1;

if(semop(semid, &mybuf, 1) < 0){
printf("Can\'t wait for condition\n");
exit(-1);
}

printf("Program 1 was spawn %d times, program 2 - %d times, total - %d times\n",
array[0], array[1], array[2]);

if(shmdt(array) < 0){
printf("Can't detach shared memory\n");
exit(-1);
}
return 0;
}