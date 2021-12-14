#include "header.h"

int main(){
    pid_t pid;
    int status;

    //printf("Avvio server %d...\n",getpid());

    //KEY SHM - QUEUE
    key_t shm_key = ftok(".",'a');
    key_t queue_key = ftok(".",'b');
    key_t sem_key = ftok(".",'c');

    printf("Avvio server %d...   [queue key: %d]\n",getpid(),queue_key);

    //Creazione SHM
    int shm_id = shmget(shm_key,sizeof(struct buffer),IPC_CREAT|0664);
    /*if(shm_id<0){
        shm_id = shmget(shm_key,sizeof(msg),0664);
    }*/

    //Definizione puntatore a SHM
    struct buffer* p = (struct buffer*)shmat(shm_id,NULL,0);

    int sem_id = semget(sem_key,1,IPC_CREAT|0664);
    /*
    //Creazione mutex
    int sem_id = semget(sem_key,1,IPC_CREAT|IPC_EXCL|0664);
    if(sem_id<0){
        sem_id = semget(sem_id,1,0664);
    }*/
    
    printf("Sem ID SERVER <%d>\n",sem_id);

    int queue_id = msgget(queue_key,IPC_CREAT|0664);
    /*
    //Creazione coda messaggi
    int queue_id = msgget(queue_id,IPC_CREAT|IPC_EXCL|0664);
    if(queue_id<0){
        queue_id = msgget(queue_key,0664);
    }*/

    printf("Coda ID SERVER <%d>\n",queue_id);
    
    int i;
    for(i=0;i<4;i++){
        receive_msg(queue_id,p,sem_id);
    }
    
    return 0;
}