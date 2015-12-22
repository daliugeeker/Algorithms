/*
 * NonBlockingConcurrentQueue.c
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
	Node * volatile node = (Node*)malloc(sizeof(Node));
	Pointer* volatile pointer = (Pointer*)sizeof(Pointer);
	Q = (Queue*)malloc(sizeof(Queue));
	pointer->Tag = 0;
	pointer->Ptr = node;
	node->Value = 0;
	node->Prev = NULL;
	node->Next = NULL;

	Q->Longth = 0;
	Q->Head = pointer;
	Q->Tail = pointer;
//	Q->Head->Tag = 0;
//	Q->Tail->Tag = 0;
//	Q->Head->Ptr = node;
//	Q->Tail->Ptr = node;
	return Q;
}

void enqueue(Queue * volatile Q, DataType val){
	if(Q == NULL){
		printf("No Queue exists!");
		exit(1);
	}
	Pointer * volatile tailPointer = (Pointer*)malloc(sizeof(Pointer));
	Node * volatile tailNode = (Node*)malloc(sizeof(Node));
	Pointer * volatile newPointer = (Pointer*)malloc(sizeof(Pointer));
	Node * volatile newNode = (Node*)malloc(sizeof(Node));

	newPointer->Tag = 10;
	newPointer->Ptr = newNode;
	newNode->Value = val;
	newNode->Next = newNode->Prev = NULL;

/* The standard enqueue operation.
 *
	tailPointer = Q->Tail;
	tailNode = tailPointer->Ptr;
	newNode->Next = tailPointer;
	tailNode->Prev = newPointer;
	Q->Tail = newPointer;
	Q->Longth++;
*/
//	CAS enqueue operation.
	while(TRUE){
		tailPointer = Q->Tail; //Read the tail pointer.
		tailNode = tailPointer->Ptr; //Read the tail node.
		newNode->Next->Tag++; //The tag of new node's next pointer is set to be one greater than the current tag of the tail pointer.
		newNode->Next = tailPointer;	
		if(__sync_bool_compare_and_swap(&(Q->Tail->Ptr),tailNode,newNode)){
			tailPointer->Tag++;
			Q->Longth++;
			tailNode->Prev = newPointer;
			break;
		}
	}
}


