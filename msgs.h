#include "sem.h"
#define MAX 1000

/* This implementation of msgs.h follows strategy 2. We use :-  
	a) mutex semaphore per port.
	b) producer semaphore per port.
	c) consumer semaphore per port. 
*/

typedef struct Mesg
{
	int data;
	int srcport;
} Mesg_t, *Mesg_p;

typedef struct Port
{
	Mesg_t mesg[10];
	int in;
	int out;
	struct Semaphore_t* prod;
	struct Semaphore_t* cons;
	struct Semaphore_t* mutex1;
} Port_t, *Port_p;

struct Semaphore_t* mutex;

/* Args - 
	id = client/server ID
	flag = specifies the direction of message flow (1 -> client to server, 0 -> server to client)
	srcport = sourceport/replyport on which sender will expect a reply. 
	dstport = destination port on which to send a message (only for printing purposes) 
	data = the data to be sent in the message (integer in this implementation)
	Port_p p = the port structure of the destination port 
*/
void Send(int id, int flag, int srcport, int dstport, int data, Port_p p)
{
	P(p->prod);
	P(p->mutex1);
	p->mesg[p->in].data = data;   // Add data to port
	p->mesg[p->in].srcport = srcport; // Setting source port on which to expect a reply
	if (flag)
		printf("Client %d sending data -> %d from source port : %d to dest port : %d in Message slot %d\n", id, data, srcport, dstport, p->in);
	else
		printf("Server %d sending data -> %d from source port : %d to dest port : %d in Message slot %d\n", id, data, srcport, dstport, p->in);
	p->in = (p->in + 1) % 10;
	V(p->mutex1);
	V(p->cons);
}


/* Args - 
	id = client/server ID
	flag = specifies the direction of message flow (1 -> server receives, 0 -> client receives) 
	port = the port number on which to receive (only for printing purposes)
	Port_p p = the port structure from which to extract message. 
*/
Mesg_t Receive(int id, int flag, int port, Port_p p)
{
	P(p->cons);
	P(p->mutex1);
	int data = p->mesg[p->out].data; // Extracting data from port
	int srcport = p->mesg[p->out].srcport; // Extracting source port on which to send a reply
	if(flag)
		printf("Server %d received data -> %d on port %d from Message slot %d\n", id, data, port, p->out);
	else
		printf("Client %d received data -> %d on port %d from Message slot %d\n", id, data, port, p->out);
	p->out = (p->out + 1) % 10;
	V(p->mutex1); 
	V(p->prod);
	Mesg_t recv_msg; 
	recv_msg.data = data;
	recv_msg.srcport = srcport;
	return recv_msg;
}
