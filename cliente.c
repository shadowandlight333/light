#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>
#include <stdio.h>

#include "claves/claves.h"
#include "mensaje.h"

#define MQ_CLIENT "/MQ_CLIENT"
#define MQ_SERVER "/MQ_SERVER"

typedef struct {
    int result;
} response;

int main(void) {

    // WE OPEN THE QUEUES
    struct mq_attr attributes;
    struct message msg;
    response myresponse;
    struct mq_attr clientAttributes = {
        .mq_flags = 0,
        .mq_maxmsg = 10,
        .mq_msgsize = sizeof(myresponse),
        .mq_curmsgs = 0
    };
    
    mqd_t q_server = mq_open(MQ_SERVER, O_WRONLY);
    mqd_t q_client = mq_open(MQ_CLIENT, O_CREAT | O_RDONLY, 0700, &clientAttributes);

    if (q_client == -1) {
        perror("mq_open(q_client)");
        return -1;
    }

    if (q_server == -1) {
        mq_close(q_client);
        perror("mq_open(q_server)");
        return -1;
    } 

    msg.op = 1;
    //struct message msg = {.key = 1, .key2 = 0, .value1 = "hola", .value2 = 87, .value3 = 5.2};

    if (mq_send(q_server, (const char *)&msg, sizeof(msg), 0) < 0) {
        perror("mq_send(q_server)");
        return -1;
    }

    mq_receive(q_client, (char *)&myresponse, sizeof(myresponse), 0);

    // if (mq_receive(q_client, (char *)&myresponse, sizeof(myresponse), 0); < 0) {
    //     perror("mq_receive(q_client)");
    //     return -1;
    // }

    printf("Result received: %d", myresponse.result);
    mq_close(q_server);
    mq_close(q_client);
    mq_unlink(MQ_CLIENT);
}