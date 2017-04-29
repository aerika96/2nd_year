#include <stdio.h>
#include <unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "a2_helper.h"
typedef struct ThreadTag{
    long int process;
    long int thread;
}ThreadT;
int sem_nr=0;
int sem_id;
int started=0;
int finished=0;
int count=0;
void P(int semId, int semNr)
{
    struct sembuf op = {semNr, -1, 0};

    semop(semId, &op, 1);
}
void P2(int semId, int semNr)
{
    struct sembuf op = {semNr, -4, 0};

    semop(semId, &op, 1);
}
void P43 (int semId, int semNr)
{
    struct sembuf op = {semNr, -43,0};

    semop(semId, &op, 1);
}
void V(int semId, int semNr)
{
    struct sembuf op = {semNr, +1, 0};

    semop(semId, &op, 1);
}
void V3(int semId, int semNr)
{
    struct sembuf op = {semNr, +3, 0};

    semop(semId, &op, 1);
}
void V43(int semId, int semNr)
{
    struct sembuf op = {semNr, +43, 0};

    semop(semId, &op, 1);
}
void thread_function3(void*argument){
    ThreadT* thr_struc = (ThreadT*)argument;
    int thr = thr_struc->thread;
    int proc = thr_struc->process;
    if(thr==3) {
        P(sem_id,7);
        info(BEGIN, proc, thr);
        info(END, proc, thr);
    }
    else{
        info(BEGIN, proc, thr);
        info(END, proc, thr);
    }
    if(thr==4){
        V(sem_id,6);
    }

}

void thread_function6(void*argument){
    ThreadT* thr_struc = (ThreadT*)argument;
    int thr = thr_struc->thread;
    int proc = thr_struc->process;

    P(sem_id,2);
    info(BEGIN,proc,thr);
    V(sem_id,3);
    P(sem_id,5);
    count++;
    V(sem_id,5);//+1 to end 15
    if(thr==15){
        started=1;
        P2(sem_id,3);
       // P2(sem_id, 3);
        info(END, proc, 15);
        finished=1;
        V3(sem_id,4);
        V3(sem_id,3);
    }
    if(thr!=15){

        if(finished  || count < 40) {
            P(sem_id, 3);//-1 to end 15
            info(END, proc, thr);
        }
        else {
            P(sem_id,4);
            P(sem_id, 3);
            info(END, proc, thr);
        }
    }
    V(sem_id,2);
}

void thread_function8(void *argument)
{

    ThreadT* thr_struc = (ThreadT*)argument;
    int thr = thr_struc->thread;
    int proc = thr_struc->process;
    if(thr!=1 && thr!=4){
        if(thr==3) {

            P(sem_id,6);
            info(BEGIN,proc,thr);
            info(END, proc, thr);
            V(sem_id,7);
        }
        else{

            info(BEGIN,proc,thr);
            info(END, proc, thr);
        }

    }
    else {
        if(thr==4){
            info(BEGIN,proc,thr);
            V(sem_id,1);
            P(sem_id,0);
            info(END,proc,thr);
        }
        else if(thr==1){
            P(sem_id,1);
            info(BEGIN,proc,thr);
            info(END,proc,thr);
            V(sem_id,0);
        }
    }
}

int main(int argc, char **argv){
    init();
    int childPid,state;
    ThreadT arguments[60];
    pthread_t th8[4];
    pthread_t th6[43];
    pthread_t th3[5];
    sem_id = semget(IPC_PRIVATE, 8, IPC_CREAT | 0600);
    if (sem_id < 0) {
        perror("Error creating the semaphore set");
        exit(2);
    }
    semctl(sem_id, 0, SETVAL, 0);//P8
    semctl(sem_id,1,SETVAL,0);//P8
    semctl(sem_id,2,SETVAL,4);//P6
    semctl(sem_id,3,SETVAL,0);//P6
    semctl(sem_id,4,SETVAL,0);//P6
    semctl(sem_id,5,SETVAL,1);//P6
    semctl(sem_id,6,SETVAL,0);
    semctl(sem_id,7,SETVAL,0);
    info(BEGIN, 1, 0);
    childPid=fork(); // P1
    switch(childPid) {
        case -1:
            exit(1);
        case 0:
            // P2
            info(BEGIN,2,0);
            childPid = fork();
            switch(childPid) {
                case -1:
                    exit(1);
                case 0:
                    //P3
                    info(BEGIN, 3, 0);

                    childPid = fork();
                    switch (childPid) {
                        case -1:
                            exit(1);
                        case 0:
                            //P6
                            info(BEGIN, 6, 0);
                            for(int i=0; i<43; i++){
                                (arguments+i)->thread=i+1;
                                (arguments+i)->process=6;
                                if(pthread_create(&th6[i],NULL,(void *(*)(void*))thread_function6,&arguments[i])){
                                    perror("Cannot create thread");
                                    exit(1);
                                }
                            }
                            for(int i=0;i<43; i++){
                                pthread_join(th6[i],NULL);
                            }
                            childPid = fork();
                            switch (childPid) {
                                case -1:
                                    exit(1);
                                case 0:
                                    //P9
                                    info(BEGIN, 9, 0);
                                    info(END, 9, 0);
                                    exit(0);
                                default:
                                    //P6
                                    wait(&state);
                                    info(END, 6, 0);
                                    exit(0);
                            }
                        default:
                            //P3

                            childPid = fork();
                            switch (childPid) {
                                case -1:
                                    exit(1);
                                case 0:
                                    //P8
                                    info(BEGIN, 8, 0);
                                    for(int i=0; i<4; i++){
                                        (arguments+i)->thread=i+1;
                                        (arguments+i)->process=8;
                                        if(pthread_create(&th8[i],NULL,(void *(*)(void*))thread_function8,&arguments[i])){
                                            perror("Cannot create thread");
                                            exit(1);
                                        }
                                    }
                                    for(int i=0;i<4; i++){
                                        pthread_join(th8[i],NULL);
                                    }
                                    info(END, 8, 0);
                                    exit(0);
                                default:
                                    //P3
                                    for(int i=0; i<5; i++){
                                        (arguments+i)->thread=i+1;
                                        (arguments+i)->process=3;
                                        if(pthread_create(&th3[i],NULL,(void *(*)(void*))thread_function3,&arguments[i])){
                                            perror("Cannot create thread");
                                            exit(1);
                                        }
                                    }
                                    wait(&state);
                                    wait(&state);

                                    for(int i=0;i<5; i++){
                                        pthread_join(th3[i],NULL);
                                    }
                                    info(END, 3, 0);
                                    exit(0);;

                            }
                    }

                default:
                    //P2
                    childPid = fork();
                    switch (childPid) {
                        case -1:
                            exit(1);
                        case 0:
                            //P4
                            info(BEGIN,4,0);
                            childPid = fork();
                            switch (childPid) {
                                case -1:
                                    exit(1);
                                case 0:
                                    //P5
                                    info(BEGIN, 5, 0);
                                    info(END, 5, 0);
                                    exit(0);
                                default:
                                    //P4
                                    wait(&state);
                                    info(END, 4, 0);
                                    exit(0);
                            }
                        default:
                            //P2
                            childPid = fork();
                            switch (childPid) {
                                case -1:
                                    exit(1);
                                case 0:
                                    //P7
                                    info(BEGIN, 7, 0);
                                    info(END, 7, 0);
                                    exit(0);
                                default:
                                    //P2
                                    wait(&state);
                                    wait(&state);
                                    wait(&state);
                                    info(END, 2, 0);
                                    exit(0);

                            }
                    }
            }
        default:;
    }
    wait(&state);
    info(END,1,0);
    return 0;
}
