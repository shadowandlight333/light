/*el servidor almacena y gestiona las claves*/

#include <stdbool.h>
#include <mqueue.h>
#include "mensaje.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "claves/claves.h"
#define MQ_SERVER "/MQ_SERVER"
#define MQ_CLIENT "/MQ_CLIENT"

mqd_t q_server;
pthread_mutex_t mutex_mess;
pthread_cond_t cond_var;
int not_copied_message = true;

typedef struct {
    int result;
} response;

void treat_message(void *mess) {
    struct message message;
    mqd_t q_client;
    response myresponse;

    struct mq_attr clientAttributes = {
        .mq_flags = 0,
        .mq_maxmsg = 10,
        .mq_msgsize = sizeof(myresponse),
        .mq_curmsgs = 0
    };

    pthread_mutex_lock(&mutex_mess);

    message = (*(struct message *) mess);
    not_copied_message = false;

    pthread_cond_signal(&cond_var);

    pthread_mutex_unlock(&mutex_mess);

    switch(message.op) {
        case 1:
            myresponse.result = 23;
            break;
        default:
            myresponse.result = 0;
            break;
    }
    myresponse.result = 23;

    printf("result sent is %d\n", myresponse.result);

    q_client = mq_open(
        MQ_CLIENT,
        O_WRONLY,
        0700,
        &clientAttributes
    );
	if (q_client == -1){
		perror("No se puede abrir la cola del cliente");
		mq_close(q_server);
		mq_unlink("/MQ_SERVER");
	}
	else {
		if (mq_send(q_client, (const char *) &myresponse, sizeof(myresponse), 0) < 0) {
			perror("mq_send");
			mq_close(q_server);
			mq_unlink("/MQ_SERVER");
			mq_close(q_client);
		}
	}
	pthread_exit(0);
}
int main(void) {
    struct message mess;
    pthread_attr_t thread_attr;
    pthread_t thread_id;
    struct mq_attr attributes = {
        .mq_flags = 0,
        .mq_maxmsg = 10,
        .mq_msgsize = sizeof(mess),
        .mq_curmsgs = 0
    };


    q_server = mq_open(MQ_SERVER, O_CREAT | O_RDWR, 0700, &attributes);
    if (q_server == -1) {
        perror("mq_open");
        return -1;
    }

    pthread_mutex_init(&mutex_mess, NULL);
 	pthread_cond_init(&cond_var, NULL);
	pthread_attr_init(&thread_attr);
    
    while (1) {
        if (mq_receive(q_server, (char *)&mess, sizeof(mess), 0) < 0) {
            perror("mq_receive");
            return -1;        
        }
        printf("Recibido mensaje con operacion %d\n", mess.op);
        if (pthread_create(&thread_id ,&thread_attr, (void *)treat_message, (void *)&mess) == 0) {
            pthread_mutex_lock(&mutex_mess);
            while (not_copied_message)
                pthread_cond_wait(&cond_var, &mutex_mess);
            pthread_mutex_unlock(&mutex_mess);
        }
    }
    mq_close(q_server);
    mq_unlink(MQ_SERVER);
    return 0;
}
