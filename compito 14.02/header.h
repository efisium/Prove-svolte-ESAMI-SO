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
#include <sys/shm.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <sys/queue.h>
#include <sys/msg.h>
#include <sys/types.h>
#include "semafori.h"

#define NUM_STUDENTI 10
#define STR_LEN 8
#define MUTEXL 0
#define SYNCH 1

typedef struct{
    char stringa[STR_LEN+1];
    int num_studenti;
    int num_lettori;
}Buffer;

void aggiorna(Buffer* p,int sem);
void azzera(Buffer* p,int sem);
void prenota(Buffer* p,int sem);


#endif