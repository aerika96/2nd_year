#include <stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
int count=0;
int M;
int nr=0;
int N;
pthread_mutex_t mutex= PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
void increment(void *argument)
{

    long int th_id=*((long int*) argument);
    long long aux;
    for(int i=1; i<=M; i++) {
        pthread_mutex_lock(&mutex);
        while (nr != th_id) {
            pthread_cond_wait(&cond, &mutex);
        }
        aux = count;
        aux++;
        usleep(100);
        count = aux;
        printf("count = %d from thread %d \n",count,th_id);

        nr++;
        if(nr==N){
            nr=0;
        }
        pthread_mutex_unlock(&mutex);


        pthread_mutex_lock(&mutex);
        pthread_cond_broadcast(&cond);

        pthread_mutex_unlock(&mutex);
    }
    }


int main(int argc, char** argv) {
    N = atoi(argv[1]);
    M= atoi(argv[2]);
    long long *arguments= (long long *)malloc(N*sizeof(long long));
    pthread_t * th = (pthread_t*)malloc(N* sizeof(pthread_t));
    for(int i=0; i<N; i++){
        arguments[i]=i;
       if(pthread_create(&th[i],NULL,(void*(*)(void*))increment,&arguments[i])) {
           perror("Cannot create thread");
           exit(1);
       }

    }

   for(int i=0; i<N; i++){
       pthread_join(th[i],NULL);
   }
    return 0;
}