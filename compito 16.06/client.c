#include "header.h"

int main(){

    //MSG QUEUE
    key_t queue_key = ftok(".",'a');
    int queue = msgget(queue_key,IPC_CREAT|0664);
    printf("Avvio processo CLIENT [ID_QUEUE: %d]...\n",queue);
    Message msg;
    msg.type = getpid();
    Service_msg s_msg,r_msg;
    s_msg.type = RQS_TO_SND;
    s_msg.PID = getpid();
    sleep(2);
    for(int k=0;k<4;k++){
        msgsnd(queue,&s_msg,sizeof(Service_msg)-sizeof(long),0);
        printf("[%d] RQS_TO_SND sent.\n",getpid());
        msgrcv(queue,&r_msg,sizeof(Service_msg)-sizeof(long),getpid(),0);
        if(r_msg.return_type == OK_TO_SND){
            printf("[%d] OK_TO_SEND received.\n",getpid());
            for(int i=0;i<DIM_VETT;i++){
                srand(time(NULL)*i*getpid());
                msg.vett[i] = rand()%11;
            }
            msgsnd(queue,(void*)&msg,sizeof(Message)-sizeof(long),0);
            printf("[%d] Message sent. <%d> <%d>\n",getpid(),msg.vett[0],msg.vett[1]);
        }
    }

    return 0;
}