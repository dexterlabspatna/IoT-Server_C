#include "mqueue.h"
#include "mqttsrc.h"
volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt){
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message){
    int i;
    char* payloadptr;
	payloadptr = message->payload;
	mqd_t q = mq_open("/ytm", O_RDWR);
	
    for(i=0; i<message->payloadlen; i++)
    {
        putchar(*payloadptr++);
    }

    putchar('\n');
	mq_send(q,message->payload, sizeof(message->payload), 1);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause){
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}