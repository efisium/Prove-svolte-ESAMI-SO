#include "header.h"

int main(){
    pid_t pid;
    int status;

    

    //KEY QUEUE
    key_t queue_key = ftok(".",'b');

    printf("Avvio client %d...   [queue key: %d]\n",getpid(),queue_key);

    int queue_id = msgget(queue_key,IPC_CREAT|0664);
    /*

    //Creazione coda messaggi
    int queue_id = msgget(queue_id,IPC_CREAT|IPC_EXCL|0664);
    if(queue_id<0){
        queue_id = msgget(queue_key,0664);
    }*/
    printf("Coda ID CLIENT <%d>\n",queue_id);
    

    int val,i;
    for(i=0;i<4;i++){
        srand(getpid()*time(NULL));
        val = 1+rand()%2;
        sleep(val);
        send_msg(queue_id);
    }
    
    return 0;
}