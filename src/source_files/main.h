#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <MQTTClient.h>
#include <mysql.h>

#include "sqlsrc.h"
#include "mqttsrc.c"
#include <pthread.h>
#include <unistd.h>

#define ADDRESS     "tcp://52.204.244.0:1883"
#define CLIENTID    "ExampleClientSub"
#define TOPIC       "data"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000Ls


