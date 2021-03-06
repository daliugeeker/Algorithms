/*
 * Double_NonBlocking_Concurrent_Queue_Operations.c
 *
 *  Created on: Dec 9, 2015
 *      Author: liuda
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stddef.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

typedef int DataType;

typedef struct pointer_t{
	struct node_t * volatile Ptr;
	unsigned int Tag;
}Pointer;

typedef struct node_t{
	DataType Value;
	struct pointer_t * volatile Next;
	struct pointer_t * volatile Prev;
}Node;

typedef struct queue_t{
	Pointer * volatile Tail;
	Pointer * volatile Head;
	int Longth;
}Queue;

Queue * initializeQueue(Queue * volatile Q){
	Pointer* volatile pointer = (Pointer*)sizeof(Pointer);
	Node * volatile node = (Node*)malloc(sizeof(Node));
	Q = (Queue*)malloc(sizeof(Queue));
	pointer->Tag = 0;
	pointer->Ptr = node;
	node->Value = -100;
	node->Prev = NULL;
	node->Next = NULL;

	Q->Longth = 0;
	Q->Head = pointer;
	Q->Tail = pointer;
//	Q->Head->Tag = 0;
//	Q->Tail->Tag = 0;
//	Q->Head->Ptr = node;
//	Q->Tail->Ptr = node;
	if(!Q->Head || !Q->Tail)
		exit(1);
	else{
		printf("initializeQueue success!");
		return Q;
	}
}


Pointer * volatile newPointer;
Node * volatile newNode;
void enqueue(Queue * volatile Q, DataType val, int SleepTime){
	if(Q == NULL){
		printf("No Queue exists!");
		exit(1);
	}
	Pointer * volatile tailPointer = (Pointer*)malloc(sizeof(Pointer));
	Node * volatile tailNode = (Node*)malloc(sizeof(Node));
	newPointer = (Pointer*)malloc(sizeof(Pointer));
	newNode = (Node*)malloc(sizeof(Node));

	newPointer->Tag = 0;
	newPointer->Ptr = newNode;
	newNode->Value = val;
	newNode->Next = newNode->Prev = NULL;
//	newNode->Next->Ptr = newNode->Prev->Ptr = NULL;
//	newNode->Next->Tag = newNode->Prev->Tag = NULL;

/* The standard enqueue operations.
 *
	tailPointer = Q->Tail;
	tailNode = tailPointer->Ptr;
	newNode->Next = tailPointer;
	tailNode->Prev = newPointer;
	Q->Tail = newPointer;
	Q->Longth++;

	free(tailNode);
	free(tailPointer);
*/

//	CAS enqueue operations.
	while(TRUE){
		tailPointer = Q->Tail; //Read the tail pointer, with the tag is 0.
		tailNode = tailPointer->Ptr; //Read the tail node.
		newNode->Next->Ptr = tailPointer->Ptr; //Store the new node's next pointer.
		newNode->Next->Tag = tailPointer->Tag+1; //The tag of new node's next pointer is set to be 1, and 1 greater than the current tag of the tail pointer.

		sleep(SleepTime); //Hang the thread up for 10 seconds. And the ABA problem would occur at this circumstance.

		if(tailPointer == Q->Tail){
			if(__sync_bool_compare_and_swap(&(Q->Tail),&tailPointer,&newPointer) && (Q->Tail->Tag == tailPointer->Tag)){
				tailPointer->Tag++; //The tag is incremented to be 1.
				Q->Longth++; //The length of Queue is incremented by 1.
				tailNode->Prev = newPointer; //In fact, the ABA problem would also occur at this step.
				tailNode->Prev->Tag = newNode->Next->Tag; //Set all the tags of the operated pointers to be 1, as to be same.
				break;
			}
		}
		free(tailNode);
		free(tailPointer);
	}
	free(newNode);
	free(newPointer);
}

pthread_t thread[2];
Queue * Qu;

void * thread1(){
	printf("Enqueue Operation One.\n");
	enqueue(Qu, 13, 10);
	return NULL;
}

void * thread2(){
	printf("Enqueue Operation Tow.\n");
	enqueue(Qu, 17, 0);
	return NULL;
}


int main(){
	Qu = initializeQueue(Qu);

	int ret1, ret2;
	ret1 = pthread_create(&thread[0],NULL,thread1,NULL);
	ret2 = pthread_create(&thread[1],NULL,thread2,NULL);
	printf("Thread one runs:%d\n",ret1);
	printf("Thread two runs:%d\n",ret2);

//	free();
	return 0;
}


