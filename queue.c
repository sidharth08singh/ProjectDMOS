#include<stdio.h>
#include<stdlib.h>

struct node {
	int value;
	struct node* llink;
	struct node* rlink;
};

struct node* head;

struct node* createNode(int val) {
	struct node* newnode = (struct node*)malloc(sizeof(struct node));
	newnode->value = val;
	newnode->llink = NULL;
	newnode->rlink = NULL;
	return newnode;
} 

// add to rear of the queue
void insert(int val) {
	struct node* newnode = createNode(val);
	struct node* first;
	struct node* last;
	if(head == NULL) {
		// inserting first element to an empty queue
		newnode->llink = newnode;
		newnode->rlink = newnode;
		head = newnode;
		printf("Added first element\n");
	}

	// adding to rear of the queue
	else {
		newnode->llink = head->llink;
		head->llink->rlink = newnode;
		head->llink = newnode;
		newnode->rlink = head;
		printf("Added element\n");
	}

	//return head;	
}

// Remove from front of the queue
void delete() {
	struct node* temp;
	if(head == NULL) {
		printf("Queue is Empty\n");
	}

	else if (head->llink == head && head->rlink == head) {
		// Only one node present
		temp = head;
		free(temp);
		head = NULL;
		printf("Deleting last element\n");
	}

	else {
		// More than one node present
		temp = head;
		head = head->rlink;
		head->llink = temp->llink;
		temp->llink->rlink = head;
		free(temp);	
	}
	//return head;
}

struct node* rotateQ() {
	if(head != NULL)
		head = head->rlink;
}
			
// print the elements of the queue
void print() {
	if (head != NULL) {
		struct node* cur;
		cur = head; 
		while(cur->rlink != head)
		{
			printf("%d -> ", cur->value);
			cur = cur->rlink;
		}
		printf("%d\n", cur->value);
	}
	else {
		printf("Queue is Empty\n");
	}
}

int main() {
	head = NULL;
	int ch, val;
	while(1) {
		printf("1 : Insert\n2 : Delete\n3 : Print\n\n");
		printf("Choice : ");
		scanf("%d", &ch);
		printf("\n");
		switch(ch) {
			case 1: {
				printf("Enter value: ");
				scanf("%d", &val);
				printf("\n");
				insert(val);
				break;
			}
			
			case 2: { 
				delete();
				break;
			}
			
			case 3: {
				print();
				break;
			}	
	
			case 4: {
				rotateQ();
				break;
			}
				
			default: {
				if(ch == 5) { 
					exit(1);
				}
				else {
					printf("Pick Again!\n");
				}
			}	
		}
	}
	return 1;
}
