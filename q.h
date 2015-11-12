/*
Authors         :- Avijit Singh Vishen, Shankar Krishnamurthy, Sidharth Singh
Filename        :- q.h 
Description     :- This file implements a circular queue using a doubly linked list
*/

#include "TCB.h"

struct node {
	TCB_t tcb;
	struct node* llink;
	struct node* rlink;
};


void InitQ(struct node** head) {
	*head = NULL;
}

struct node* createNode(TCB_t tcb) {
	struct node* newnode = (struct node*)malloc(sizeof(struct node));
	newnode->tcb = tcb;
	newnode->llink = NULL;
	newnode->rlink = NULL;
	return newnode;
} 

// add to rear of the queue
void AddQ(struct node** head, TCB_t* tcb) {
	struct node* newnode = createNode(*tcb);
	struct node* first;
	struct node* last;
	if(*head == NULL) {
		// inserting first element to an empty queue
		newnode->llink = newnode;
		newnode->rlink = newnode;
		*head = newnode;
		//printf("Added first element\n");
	}

	// adding to rear of the queue
	else {
		newnode->llink = (*head)->llink;
		(*head)->llink->rlink = newnode;
		(*head)->llink = newnode;
		newnode->rlink = *head;
		//printf("Added element\n");
	}

	//return head;	
}

// Remove from front of the queue
TCB_t* DelQ(struct node** head) {
	struct node* temp;
	TCB_t* del_tcb;
	if(*head == NULL) {
		printf("Queue is Empty\n");
	}

	else if ((*head)->llink == *head && (*head)->rlink == *head) {
		// Only one node present
		temp = *head;
		del_tcb = &(temp->tcb);
		free(temp);
		*head = NULL;
		//printf("Deleting last element\n");
		return del_tcb;
	}

	else {
		// More than one node present
		temp = *head;
		del_tcb = &(temp->tcb);
		*head = (*head)->rlink;
		(*head)->llink = temp->llink;
		temp->llink->rlink = *head;
		free(temp);	
		return del_tcb;
	}
}

void RotateQ(struct node** head) {
	if(*head != NULL)
		*head = (*head)->rlink;
}
			
