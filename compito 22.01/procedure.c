#include "header.h"

void send_msg(int queue){
    int val;
    msg message;
    message.type = MSG_TPYE;
    srand(getpid()*time(NULL));
    for(int i=0;i<N_VAL;i++){
        val = rand()%10;
        message.val[i] = val;
    }

    //Invio messaggio
    msgsnd(queue,(void*)&message,sizeof(msg)-sizeof(long),IPC_NOWAIT);
    printf("MESSAGGIO INVIATO! MSG [1] %d    [2] %d         ...Processo <%d>\n",message.val[0],message.val[1],getpid());
}

void receive_msg(int queue,struct buffer* p,int mutex){
    msg message;
    msgrcv(queue,(void*)&message,sizeof(msg)-sizeof(long),MSG_TPYE,0);
    printf("MESSAGGIO RICEVUTO! MSG [1] %d    [2] %d         ...Processo <%d>\n",message.val[0],message.val[1],getpid());
    sleep(1);
    Wait_Sem(mutex,0);
    printf("Valori buffer (by %d)\n",getpid());
    for(int i=0;i<N_VAL;i++){
        p->val[i] = message.val[i];
        printf("[%d] %d\n",i,p->val[i]);
    }

    Signal_Sem(mutex,0);
}