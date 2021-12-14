#include "header.h"

void *start_server(void *g){
    
    Server_s *s = (Server_s*)g;
    int n = s->n;
    printf("Avvio thread %d...\n",n);
    int msg_pid;
    Service_msg r_msg;
    Service_msg s_msg;
    Message msg;
    int sum;

    for(int k=0;k<6;k++){
        sum = 0;
        //RQS_TO_SEND
        printf("%d Attendo...\n",s->n);
        msgrcv(s->queue,&r_msg,sizeof(Service_msg)-sizeof(long),RQS_TO_SND,0);
        msg_pid = r_msg.PID;
        printf("[SERVER %d] RQS_TO_SEND received from client <%d>\n",n,msg_pid);

        //OK_TO_SEND
        s_msg.return_type = OK_TO_SND;
        s_msg.type = msg_pid;
        msgsnd(s->queue,&s_msg,sizeof(Service_msg)-sizeof(long),0);
        printf("[SERVER %d] OK_TO_SEND sent to client <%d>\n",s->n,msg_pid);

        //MSG_RCV
        msgrcv(s->queue,&msg,sizeof(Message)-sizeof(long),msg_pid,0);
        printf("[SERVER %d] Message received from client <%d>: <%d> <%d> \n",n,msg_pid,msg.vett[0],msg.vett[1]);
        
        //SUM & PRINTF
        for(int i=0;i<DIM_VETT;i++){
            sum = sum + msg.vett[i];
        }
        printf("[SERVER %d] Valore %d generato da %d.\n",n,sum,msg_pid);
    }
    pthread_exit(NULL);
}

int main(){
    Server_s *s = malloc(sizeof(Server_s));

    //MSG QUEUE
    key_t queue_key = ftok(".",'a');
    int queue = msgget(queue_key,IPC_CREAT|0664);
    s->queue = queue;
    printf("Avvio processo SERVER [ID_QUEUE: %d]...\n",s->queue);
    //THREADS
    pthread_t thread[NUM_THREAD];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

    for(int i=0;i<NUM_THREAD;i++){
        s->n = i+1;
        pthread_create(&thread[i],&attr,start_server,(void*)s);
        sleep(1);
    }

    int status;
    for(int i=0;i<NUM_THREAD;i++){
        pthread_join(thread[i],(void**)&status);
        printf("Thread CLIENT %d terminato con stato <%d>\n",i,status);
    }
    
    return 0;
}