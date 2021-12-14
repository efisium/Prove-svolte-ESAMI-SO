#ifndef _HEAD_
#define _HEAD_

#include <stdio.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <pthread.h>
#include <string.h>

#define DIM_VETT 2
#define NUM_THREAD 2
#define N_CLIENT 3
#define N_SERVER 1
#define RQS_TO_SND 1
#define OK_TO_SND 2

typedef struct{
    long type;
    int PID;
    int vett[DIM_VETT];

}Message;

typedef struct{
    long type;
    int PID;
    int return_type;
}Service_msg;

typedef struct{
    int queue;
    int n;
}Server_s;


#endif