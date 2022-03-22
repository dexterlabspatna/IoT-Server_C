//https://zetcode.com/db/mysqlc/
//gcc -I/usr/include/mysql -L/usr/include/mysql testsql1.c -o abhi -lmysqlclient
//sudo apt-get install libmysqlclient-dev 
//find /usr/ -name 'mysql.h' 

//https://www.eclipse.org/paho/index.php?page=clients/c/index.php
//gcc -I/usr/include/mysql -I/usr/local/include -L/usr/include/mysql -L/usr/local/include testsql1.c -o abhi -lmysqlclient -lpaho-mqtt3c

//gcc -I/usr/include/mysql -I/usr/local/include -L/usr/include/mysql -L/usr/local/include main.c -o run -lmysqlclient -lpaho-mqtt3c -lpthread -lrt


#include "main.h"
#include "mqueue.h"



void *MqttThread(){
	printf("MQTT\n");
	MQTTClient client;
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	int rc;
	int ch;
	MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	
	MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
	
	if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS){
		printf("Failed to connect, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}
	printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
	"Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);
	MQTTClient_subscribe(client, TOPIC, QOS);
	
	do{
		ch = getchar();
	}while(ch!='Q' && ch != 'q');
	
	MQTTClient_disconnect(client, 10000);
	MQTTClient_destroy(&client);
	
	//return rc;
	
}


void *SqlThread(){
	printf("SQL\n");
	MYSQL *con = mysql_init(NULL);
	
	    char buff[64];
    mqd_t q = mq_open("/ytm", O_RDONLY);
	
	if (con == NULL){
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}
	
	if (mysql_real_connect(con, "52.204.244.0", "channa", "!Channa2021",	NULL, 0, NULL, 0) == NULL){
		fprintf(stderr, "%s\n", mysql_error(con));
		mysql_close(con);
		exit(1);
	}
	#if 1
    while(1)    {
        int retval =  mq_receive(q,buff,sizeof(buff), NULL);
        if(retval ==  -1)   {
            //printf("read error  \n");
        } else {
            printf("got message : %s\n", buff);
        }
    }
	#endif
//	mysql_close(con);
	
}



int main(int argc, char **argv){

pthread_t thread1, thread2;
char *message1 = "Thread 1";
char *message2 = "Thread 2";
int  iret1, iret2;

    struct mq_attr attr;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 32;
    mq_unlink ("/ytm");
    mqd_t queue = mq_open("/ytm", O_CREAT | O_RDWR, 0, &attr);


iret1 = pthread_create( &thread1, NULL, MqttThread, (void*) message1);
iret2 = pthread_create( &thread2, NULL, SqlThread, (void*) message2);

pthread_join( thread1, NULL);
pthread_join( thread2, NULL); 

printf("Thread 1 returns: %d\n",iret1);
printf("Thread 2 returns: %d\n",iret2);

 while(1);


#if 0

if (mysql_query(con, "CREATE DATABASE testdb"))
{
fprintf(stderr, "%s\n", mysql_error(con));
mysql_close(con);
exit(1);
}
#endif
exit(0);
}