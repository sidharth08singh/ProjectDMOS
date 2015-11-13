/*
Authors  	:- Avijit Singh Vishen, Shankar Krishnamurthy, Sidharth Singh
Filename 	:- msgs_test.c 
Description 	:- This file spawns 3 clients and 2 servers. Message exchange happens between clients and servers. A client sends a message to the                server and gets a reply. 
*/


#include<stdio.h>
#include<stdlib.h>
#include "msgs.h"


Port_t Ports[100];         // List of 100 ports
int producerID = 0;	   // To keep track of client IDs	
int consumerID = 0; 	   // To keep track of server IDs
int portnumber = 0;	   // To assign a unique port number from 1 to 100 for each client and server
int portS1;		   // To keep track of server port 	
int portS2;

void client()
{
	int Id, portC;
	P(mutex_pid);          
	portC = portnumber % 100; // Port Numbers can be 0 through 99
	portnumber++;
	Id = producerID;
	producerID++;
	V(mutex_pid);
	int i,data;
	while(1)
	{ 
		data = rand() % 10000;	// Generate a Random Number to send. 
		if(Id % 2 == 0)
		{
			//Send() Args - Id, flag (specifies direction, 1 -> client to server, 2 -> server to client), Source Port Number, Dest Port Number, Data, Destination Port
			Send(Id, 1, portC, portS1, data, &Ports[portS1]); 
		}
		else
		{	
			Send(Id, 1, portC, portS2, data, &Ports[portS2]); 
		}
		sleep(1);
		Receive(Id, 0, portC, &Ports[portC]);
	}
}

void server()
{
	int Id, replyport, svrport;
	P(mutex_pid);
	Id = consumerID;
	consumerID++;
	if(portS1 == 0 && portS2 == 0)
	{
		portS1 = portnumber % 100; // Port Numbers can be 0 through 99 
		svrport = portS1;
		printf("#### Server %d Listening at Port Number %d ####\n", Id, svrport);
	}	
	else
	{
		portS2 = portnumber % 100; // Port Numbers can be 0 through 99
		svrport = portS2;
		printf("#### Server %d Listening at Port Number %d ####\n", Id, svrport);
	}
	portnumber++;
	V(mutex_pid);
	int i;
	Mesg_t rcv_msg;
	while(1)
	{ 
		rcv_msg = Receive(Id, 1, svrport, &Ports[svrport]); // Receive() args - Id, flag (specifies direction, 1 -> server receives, 0 -> client receives), Port Number, Port
		rcv_msg.data *= 100; //Modify Received Message & send back. 
		Send(Id, 0, svrport, rcv_msg.srcport, rcv_msg.data, &Ports[rcv_msg.srcport]);
	}
}

int main()
{
	InitQ(&RunQ);
        mutex_pid = CreateSem(1); // Semaphore to assign port numbers & id among various clients and servers. 
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
	start_thread(client);
	start_thread(client);
	start_thread(client);
        start_thread(server);
	start_thread(server);
	run();
}
