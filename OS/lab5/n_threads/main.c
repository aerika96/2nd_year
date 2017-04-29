#include <stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
char q='a';
void * th_function(void* argument){
    long int th_id=*((long int*) argument);
    while(q!='q') {
        printf("Thread with the identifier %ld is running with the tid %ld \n", (long int) pthread_self(), th_id);

    }
    sleep(1);
    printf("I reached the termination of thread %ld\n",th_id);
    return th_id;
}
int main(int argc, char**argv){
    int N =atoi(argv[1]);
    long int *arguments = (long int *)malloc(N*sizeof(long int));
    pthread_t* th = (pthread_t*)malloc(N*sizeof(pthread_t));
    for(int i=0; i<N; i++){
        arguments[i]=i+1;
        if(pthread_create(&th[i],NULL,th_function,&arguments[i])!=0){
            perror("Cannot create thread");
            exit(1);
        }
    }
    while(1) {
        scanf("%c", &q);
        if(q=='q')
            break;
    }
    for(int i=0; i<N; i++){
        pthread_join(th[i],NULL);
    }

    printf("I waited all the threads to terminate");
    free(arguments);
    free(th);
    return 0;
}