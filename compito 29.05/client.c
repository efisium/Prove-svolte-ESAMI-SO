#include "header.h"

int main(){
    printf("Avvio processo CLIENT...\n");

    //KEY
    key_t key_1 = ftok(".",'a');
    key_t key_2 = ftok(".",'b');

    int queue_1 = msgget(key_1,IPC_CREAT|0664);
    int queue_2 = msgget(key_2,IPC_CREAT|0664);

    for(int i=0;i<NUM_MESS;i++){
        msg_send(queue_1,queue_2);
    }
    return 0;
}