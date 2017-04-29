#include <stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
long long count = 0;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
void increment(void* ind)
{
    pthread_mutex_lock(&mutex);
    long long aux;
    aux = count;
    aux++;
    usleep(100);
    count = aux;
    pthread_mutex_unlock(&mutex);
}

int main(int argc, char **argv) {
    int N=atoi(argv[1]);

    long int *arguments= (long int*) malloc(N*sizeof(long int));
    pthread_t * th = (pthread_t*)malloc(N*sizeof(pthread_t));
    for(int i=0;i<N; i++){
        if(pthread_create(&th[i],NULL,(void* (*) (void*)) increment,&arguments[i])!=0){
            perror("Cannot create thread");
            exit(1);
        }

    }

    for(int i =0; i<N; i++){

        pthread_join(th[i],NULL);
    }

    printf("%d",count);
    return 0;
}