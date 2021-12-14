#ifndef _HEAD_
#define _HEAD_

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "semafori.h"
#define N_CLIENT 3
#define N_SERVER 3
#define N_VAL 2
#define MSG_TPYE 1

typedef struct{
    long type;
    int val[N_VAL];
}msg;

struct buffer{
    int val[N_VAL];
};

void send_msg(int);
void receive_msg(int,struct buffer*,int);

#endif