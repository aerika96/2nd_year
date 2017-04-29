#include <stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <unistd.h>
int thNo = 0;
int M;
pthread_mutex_t mutex= PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition=PTHREAD_COND_INITIALIZER;
void limitedArea(void)
{
    pthread_mutex_lock(&mutex);
        while(thNo>=M) {
            pthread_cond_wait(&condition, &mutex);
        }
    thNo++;
    pthread_mutex_unlock(&mutex);


    printf("The no of threads in the limited area is: %d\n", thNo);
    usleep(100);


    pthread_mutex_lock(&mutex);
    thNo--;
    pthread_cond_signal(&condition);

    pthread_mutex_unlock(&mutex);

}

int main(int argc, char ** argv) {
    int N= atoi(argv[1]);
    M=atoi(argv[2]);
    pthread_t *th = (pthread_t*)malloc(N*sizeof(pthread_t));
    for(int i=0; i<N; i++){
        if(pthread_create(&th[i],NULL,(void* (*) (void*)) limitedArea,NULL)){
            perror("Cannot create thread");
            exit(1);
        }
    }
    return 0;
}