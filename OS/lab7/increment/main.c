#include <stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
int N;
int sem_id;
long long count;
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
void increment(int th_id){
    long long aux;
    P(sem_id,0);
    aux=count;
    aux++;
    usleep(100);
    count=aux;
    V(sem_id,0);


}
int main(int argc, char**argv) {
    N= atoi(argv[1]);
    sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
    if (sem_id < 0) {
        perror("Error creating the semaphore set");
        exit(2);
    }
    semctl(sem_id, 0, SETVAL, 1);
    long int *arguments = (long int *)malloc(N*sizeof(long int));
    pthread_t *th = (pthread_t*) malloc(N*sizeof(pthread_t));
    for(int i=0; i<N; i++){
        arguments[i]=i+1;
        if(pthread_create(&th[i],NULL,(void* (*) (void*)) increment,&arguments[i])){
            perror("Cannot create thread");
            exit(1);
        }
    }
    for(int i=0; i<N; i++){
        pthread_join(th[i],NULL);
    }
    printf("%d ",count);
    return 0;
}