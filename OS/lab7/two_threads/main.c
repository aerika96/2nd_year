#include <stdio.h>
#include <stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
long long count = 0;
int sem_nr=0;
int sem_id;
void P(int semId, int semNr)
{
    struct sembuf op = {semNr, -1, 0};

    semop(semId, &op, 1);
}
void V(int semId, int semNr)
{
    struct sembuf op = {semNr, +1, 0};

    semop(semId, &op, 1);
}
void increment(void* thId)
{
    int i=0;
    while (i < 100) {
        P(sem_id,sem_nr);
        long long aux;
        aux = count;
        aux++;
        printf("count=%lld from thread %d\n", count, *(int*)thId);
        count = aux;
        if(*(int*)thId==0){
            sem_nr=1;
        }
        else{ sem_nr=0;}
        V(sem_id,sem_nr);
        i++;
    }
}
int main() {
    int one=1;
    int zero=0;
    sem_id = semget(IPC_PRIVATE, 2, IPC_CREAT | 0600);
    if (sem_id < 0) {
        perror("Error creating the semaphore set");
        exit(2);
    }
    semctl(sem_id, 0, SETVAL, 1);
    semctl(sem_id,1,SETVAL,0);
    pthread_t* th = (pthread_t*)malloc(2*sizeof(pthread_t));
    if(pthread_create(&th[0],NULL, (void * (*)(void*))increment,&zero)){
        printf("Cannot create thread");
        exit(1);
    }
    if(pthread_create(&th[1],NULL, (void* (*) (void*))increment,&one)){
        printf("Cannot create thread");
        exit(2);
    }

    pthread_join(th[0],NULL);
    pthread_join(th[1],NULL);
    return 0;
}