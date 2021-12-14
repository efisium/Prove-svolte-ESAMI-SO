#include "header.h"

int main(){
    printf("Avvio processo SERVER...\n");

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

    pthread_t thread_worker[NUM_WORKER];
    pthread_t thread_manager;

    //KEY
    key_t key_1 = ftok(".",'a');
    key_t key_2 = ftok(".",'b');

    int queue_1 = msgget(key_1,IPC_CREAT|0664);
    int queue_2 = msgget(key_2,IPC_CREAT|0664);

    Buffer *b = malloc(sizeof(Buffer));
    pthread_mutex_init(&b->mutex,NULL);
    pthread_cond_init(&b->ok_cons,NULL);
    pthread_cond_init(&b->ok_prod,NULL);

    b->queue = queue_1;
    b->queue_rcv = queue_2;
    b->testa = 0;
    b->coda = 0;
    b->cont = 0;

    for(int i=0;i<NUM_WORKER;i++){
        b->worker[i] = thread_worker[i];
    }

    //CREATE THREADS
    for(int i=0;i<NUM_WORKER;i++){
        pthread_create(&thread_worker[i],&attr,start_server_worker,(void*)b);
    }

    pthread_create(&thread_manager,&attr,start_server_manager,(void*)b);

    //JOIN THREADS
    for(int i=0;i<NUM_WORKER;i++){
        pthread_join(thread_worker[i],NULL);
    }

    pthread_join(thread_manager,NULL);

    pthread_mutex_destroy(&b->mutex);
    pthread_cond_destroy(&b->ok_cons);
    pthread_cond_destroy(&b->ok_prod);
    pthread_exit(NULL);
    return 0;
}