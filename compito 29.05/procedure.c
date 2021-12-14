#include "header.h"

void msg_send(int queue_1,int queue_2){
    Message msg;
    Message_server msg_rcv;
    msg.type = MSG_TYPE;
    msg.PID = getpid();
    for(int i=0;i<2;i++){
        srand(time(NULL)*i);
        msg.vett[i] = rand()%11;
    }
    msgsnd(queue_1,(void*)&msg,sizeof(Message)-sizeof(long),IPC_NOWAIT);
    printf("Messaggio CLIENT: PID <%d> VETT [%d] [%d]\n",msg.PID,msg.vett[0],msg.vett[1]);
    msgrcv(queue_2,(void*)&msg_rcv,sizeof(Message_server)-sizeof(long),getpid(),0);
    printf("Messaggio SERVER: Risultato <%d>\n",msg_rcv.res);
}

void* start_server_manager(void* g){
    Buffer* b = (Buffer*) g;
    Message msg;
    int stat;
    while(1){
        sleep(1);
        stat = msgrcv(b->queue,(void*)&msg,sizeof(Message)-sizeof(long),MSG_TYPE,IPC_NOWAIT);
        if(stat == -1){
            continue;
        }else{
            if(msg.vett[0] == -1 && msg.vett[1] == -1){
                printf("Mo ti termino i threads..\n");
                for(int j=0;j<NUM_WORKER;j++){
                    pthread_cancel(b->worker[j]);
                }
                pthread_exit(NULL);
            }else{
                produci(b,msg);
            }
        }
    }
}

void produci(Buffer* b,Message msg){
    pthread_mutex_lock(&b->mutex);
    if(b->cont == DIM_BUFFER){
        pthread_cond_wait(&b->ok_prod,&b->mutex);
    }

    b->buffer[b->testa] = msg;
    b->testa = (b->testa + 1) % DIM_BUFFER;

    pthread_cond_signal(&b->ok_cons);
    pthread_mutex_unlock(&b->mutex);
}

void* start_server_worker(void* g){
    Buffer* b = (Buffer*) g;
    Message msg;
    Message_server msg_rcv;
    int res;
    while(1){
        msg = consuma(b);
        res = 0;
        for(int i=0;i<2;i++){
            res = res + msg.vett[i];
        }
        msg_rcv.type = msg.PID;
        msg_rcv.res = res;
        msgsnd(b->queue_rcv,(void*)&msg_rcv,sizeof(Message_server)-sizeof(long),IPC_NOWAIT);
    }
}

Message consuma(Buffer* b){
    Message msg;
    pthread_mutex_lock(&b->mutex);
    if(b->cont == 0){
        pthread_cond_wait(&b->ok_cons,&b->mutex);
    }

    msg = b->buffer[b->coda];
    b->coda = (b->coda + 1) % DIM_BUFFER;

    pthread_cond_signal(&b->ok_prod);
    pthread_mutex_unlock(&b->mutex);
    return msg;
}