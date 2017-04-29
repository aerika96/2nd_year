#include <stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
int th_no=0;
int sem_id;
int sem_nr=0;
int M,N;
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
void limitedArea(void){
        P(sem_id, 0);
        th_no++;
        usleep(100);
        printf("The no of threads in the limited area is: %d\n", th_no);
        th_no--;
        sem_nr++;
        V(sem_id,0);


}
int main(int argc, char**argv) {
     N= atoi(argv[1]);
     M= atoi(argv[2]);
    if(M>=N){
        perror("Too many semaphores");
        exit(2);
    }
    sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
    if (sem_id < 0) {
        perror("Error creating the semaphore set");
        exit(2);
    }
    semctl(sem_id, 0, SETVAL, M);
    long int *arguments=(long int*)malloc(N*sizeof(long int));
    pthread_t *th = (pthread_t*)malloc(N*sizeof(pthread_t));
    for(int i=0; i<N; i++){
        arguments[i]=i;
        if(pthread_create(&th[i],NULL,(void* (*) (void*))limitedArea,&arguments[i])){
            perror("Cannot create thread");
            exit(1);
        }
    }
    for(int i=0;i<N; i++){
        pthread_join(th[i],NULL);
    }
    return 0;
}