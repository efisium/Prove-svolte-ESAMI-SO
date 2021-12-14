#ifndef _HEAD_
#define _HEAD_

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <sys/queue.h>
#include <sys/msg.h>
#include <sys/types.h>

#define DIM_BUFFER 3
#define NUM_MESS 5
#define NUM_CLIENT 3
#define NUM_SERVER 1
#define NUM_WORKER 2
#define NUM_MANAGER 1
#define MSG_TYPE 1

typedef struct{
    long type;
    int PID;
    unsigned int vett[2];
}Message;

typedef struct{
    long type;
    int res;
}Message_server;

typedef struct{
    Message buffer[DIM_BUFFER];
    int testa;
    int coda;
    int cont;

    int queue;
    int queue_rcv;
    pthread_t worker[NUM_WORKER];
    pthread_mutex_t mutex;
    pthread_cond_t ok_prod;
    pthread_cond_t ok_cons;

}Buffer;

void msg_send(int queue_1,int queue_2);
void* start_server_manager(void* g);
void* start_server_worker(void* g);
void produci(Buffer* b,Message msg);
Message consuma(Buffer* b);

#endif