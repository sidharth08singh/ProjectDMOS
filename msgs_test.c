#include<stdio.h>
#include<stdlib.h>
#include "msgs.h"

Port_t Ports[100];
int producerID = 0;
int consumerID = 0; 
int portnumber = 1;
int portS;
int portC;

void producer()
{
	int Id;
	P(mutex);
	portC = portnumber;
	portnumber++;
	Id = producerID;
	producerID++;
	V(mutex);
	int i,data;
	time_t t;
	srand((unsigned) time(&t));
	for(i = 0 ; i < 3; i++)
	{ 
		data = rand() % 1000;	
		Send(Id, 1, portC, portS, data, &Ports[portS]); // Args -  Id, flag (specifies direction, 1 -> client to server, 2 -> server to client), Source Port Number, Dest Port Number, Data, Destination Port
		Receive(Id, 0, portC, &Ports[portC]);
	}
}

void consumer()
{
	int Id;
	P(mutex);
	portS = portnumber;
	printf("#### Consumer %d Listening at Port Number %d ####\n", Id, portnumber);
	portnumber++;
	Id = consumerID;
	consumerID++;
	V(mutex);
	int i;
	Mesg_t rcv_msg;
	//for(i = 0; i < 3; i++)
	while(1)
	{ 
		rcv_msg = Receive(Id, 1, portS, &Ports[portS]); // Args - Id, flag (specifies direction, 1 -> server receives, 0 -> client receives) Port Number, Port
		//Modify Received Message & send back. 
		rcv_msg.data *= 100;
		Send(Id, 0,  portS, portC, rcv_msg.data, &Ports[rcv_msg.srcport]);
	}
}

int main()
{
	InitQ(&RunQ);
        mutex = CreateSem(1);
	int i,j;
	for(i=0; i<100; i++)
	{
		for(j=0; j<10; j++)
		{
			Ports[i].mesg[j].data = -1;		
			Ports[i].mesg[j].srcport = -1;
		}
		Ports[i].prod = CreateSem(10);
		Ports[i].cons = CreateSem(0);
		Ports[i].mutex1 = CreateSem(1);
		Ports[i].in   = 0;
		Ports[i].out  = 0;
	}
	start_thread(producer);
	start_thread(producer);
        start_thread(consumer);
	run();
}
