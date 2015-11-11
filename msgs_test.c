#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "msgs.h"

Port_t Ports[100];         // List of 100 ports
int producerID = 0;	   // To keep track of client IDs	
int consumerID = 0; 	   // To keep track of server IDs
int portnumber = 1;	   // To assign a unique port number from 1 to 100 for each client and server
int portS;		   // To keep track of server port 	

void producer()
{
	int Id, portC;
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
		data = rand() % 10000;	// Generate a RandoM Number to send. 
		Send(Id, 1, portC, portS, data, &Ports[portS]); //Send() Args -  Id, flag (specifies direction, 1 -> client to server, 2 -> server to client), Source Port Number, Dest Port Number, Data, Destination Port
		Receive(Id, 0, portC, &Ports[portC]);
	}
}

void consumer()
{
	int Id, replyport;
	P(mutex);
	portS = portnumber;
	printf("#### Server %d Listening at Port Number %d ####\n", Id, portnumber);
	portnumber++;
	Id = consumerID;
	consumerID++;
	V(mutex);
	int i;
	Mesg_t rcv_msg;
	while(1)
	{ 
		rcv_msg = Receive(Id, 1, portS, &Ports[portS]); // Receive() args - Id, flag (specifies direction, 1 -> server receives, 0 -> client receives), Port Number, Port
		rcv_msg.data *= 100; //Modify Received Message & send back. 
		Send(Id, 0, portS, rcv_msg.srcport, rcv_msg.data, &Ports[rcv_msg.srcport]);
	}
}

int main()
{
	InitQ(&RunQ);
        mutex = CreateSem(1); // Semaphore to assign port numbers & id among various clients and servers. 
	int i,j;
	for(i=0; i<100; i++) // Initializing all the 100 ports
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
