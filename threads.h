/*
Authors         :- Avijit Singh Vishen, Shankar Krishnamurthy, Sidharth Singh
Filename        :- threads.h 
Description     :- This file implements start_thread, yield and run functions. 
*/

#include "q.h"

struct node* RunQ;

void start_thread(void (*function)(void))
{
     //allocate a stack (via malloc) of a certain size (choose 8192)
     int *stack_ptr = (int*)malloc(8192);
     //allocate a TCB (via malloc)
     TCB_t *tcb = (TCB_t*)malloc(sizeof(TCB_t)); 
     //call init_TCB with appropriate arguments
     init_TCB(tcb, function, stack_ptr, 8192); 
     //call addQ to add this TCB into the “RunQ” which is a global header pointer
     AddQ(&RunQ, tcb);
}

void run()
{   // real code
    ucontext_t parent;     // get a place to store the main context, for faking
    getcontext(&parent);   // magic sauce
    swapcontext(&parent, &((RunQ->tcb).context));  // start the first thread
}
void yield() // similar to run
{
   //rotate the run Q;
   //printf("\nYield Happened\n");
   RotateQ(&RunQ);
   //swap the context, from previous thread to the thread pointed to by runQ
   swapcontext(&((RunQ->llink)->tcb).context, &(RunQ->tcb).context);
}
