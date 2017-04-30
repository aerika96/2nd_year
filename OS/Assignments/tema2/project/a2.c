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

//Fuction decrementing a semaphore's value with 1
void P(int semId, int semNr)
{
    struct sembuf op = {semNr, -1, 0};

    semop(semId, &op, 1);
}

//Function decrementing the semaphore's value with 4
void P4(int semId, int semNr)
{
    struct sembuf op = {semNr, -4, 0};

    semop(semId, &op, 1);
}

//Function incrementing the semaphore's value with 1
void V(int semId, int semNr)
{
    struct sembuf op = {semNr, +1, 0};

    semop(semId, &op, 1);
}

//Function incrementing the semaphore's value with 3
void V3(int semId, int semNr)
{
    struct sembuf op = {semNr, +3, 0};

    semop(semId, &op, 1);
}

//Function for the process nr 3. - synchronizing threads in other processes
void thread_function3(void*argument){

    ThreadT* thr_struc = (ThreadT*)argument;
    int thr = thr_struc->thread;
    int proc = thr_struc->process;

    //thread 3.3 must wait for process 8.3 to terminate before it starts
    // - semaphore 7 is incremented after thhread 8.3 has finished
    if(thr==3) {
        P(sem_id,7);
        info(BEGIN, proc, thr);
        info(END, proc, thr);
    }
    else{
        info(BEGIN, proc, thr);
        info(END, proc, thr);
    }
    if(thr==4)
        //thread 8.3 must only start after thread 3.4 has finished
        //-semaphore 6 incremented after thread 8.4 has terminated
        V(sem_id,6);
}


//Function for the process nr 6. - thread barrier
void thread_function6(void*argument)
{
    ThreadT* thr_struc = (ThreadT*)argument;
    int thr = thr_struc->thread;
    int proc = thr_struc->process;

    // semaphore 2 has the initial value of 4 - doesn't let more than 4 threads enter
    // each time a thread tries to enter -tries to decrement the value - incremented after finished
    P(sem_id,2);
    info(BEGIN,proc,thr);
    V(sem_id,3); //semaphore 3 is incremented each time a thread enters, decremented when has finished - initial value of 0, intended to "count how many threads are currently running"
    P(sem_id,5); //semaphore 5 's purpose is to "protect" the count global variable's incrementation - has the initial value of 1
    count++;
    V(sem_id,5);
    if(thr==15){
        started=1;
        P4(sem_id,3); //when the thread s nr 15. it tries to decrement the semaphore's value with 4 - it means 4 are running - if succeeded, terminates
        info(END, proc, 15);
        finished=1;
        V3(sem_id,4); // semaphore 4 is used to block the termination of all the threads if there are no more than 4 that hasn't been terminated yet
        V3(sem_id,3); // if thread 15 successfully terminated, it means that it also decremented the semaphore 3's value - has to be incremented, not to block the others
    }
    if(thr!=15){
        //if finished or there were less that 40 threads run -> the others can terminate with no restriction
        if(finished  || count < 40) {
            P(sem_id, 3);// increments semaphore 3's value
            info(END, proc, thr);
        }
        //in case there are only 4 threads left and 15 hasn't terminated yet -> all the threads are blocked until it does
        else {
            P(sem_id,4);
            P(sem_id, 3);
            info(END, proc, thr);
        }
    }
    V(sem_id,2);
}

//Function for process nr 8.
void thread_function8(void *argument)
{

    ThreadT* thr_struc = (ThreadT*)argument;
    int thr = thr_struc->thread;
    int proc = thr_struc->process;

    if(thr!=1 && thr!=4){
        //thread 8.3 has to wait for 3.4 to end - semaphore 6
        if(thr==3) {
            P(sem_id,6);
            info(BEGIN,proc,thr);
            info(END, proc, thr);
            V(sem_id,7); // 3.3 must only start after 8.3 has terminated - semaphore 7
        }
        else{

            info(BEGIN,proc,thr);
            info(END, proc, thr);
        }

    }
    else { // thread 8.4 has to start before 8.1 but must end only after it
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
    pthread_t th8[4]; //threads of process 8
    pthread_t th6[43]; //threads of process 6
    pthread_t th3[5]; //threads of process 3

    sem_id = semget(IPC_PRIVATE, 8, IPC_CREAT | 0600); // creating 8 semaphores - the number of semaphores I use in total
    if (sem_id < 0) {
        perror("Error creating the semaphore set");
        exit(2);
    }
    //initializing semaphores
    semctl(sem_id, 0, SETVAL, 0);//P8
    semctl(sem_id,1,SETVAL,0);//P8
    semctl(sem_id,2,SETVAL,4);//P6
    semctl(sem_id,3,SETVAL,0);//P6
    semctl(sem_id,4,SETVAL,0);//P6
    semctl(sem_id,5,SETVAL,1);//P6
    semctl(sem_id,6,SETVAL,0);//P3,P8
    semctl(sem_id,7,SETVAL,0);//P3,P8

    //process hierarchy "construction" starts here
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
