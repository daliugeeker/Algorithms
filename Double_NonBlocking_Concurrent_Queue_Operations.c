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

	Q->Longth = 1;
	Q->Head->Tag = 0;
	Q->Head = node;
	Q->Tail = node;
	return Q;
}

void enqueue(Queue * volatile Q, DataType val){
	if(Q == NULL){
		printf("No Queue exists!");
		exit(1);
	}
	Pointer * tail = (Pointer*)malloc(sizeof(Pointer));
	Pointer * newPointer = (Pointer*)malloc(sizeof(Pointer));
	Node * volatile newNode = (Node*)malloc(sizeof(Node));

	newPointer->Tag = 0;
	newPointer->Ptr = newNode;
	newNode->Value = val;
	newNode->Next = newNode->Prev = NULL;

/* The standard enqueue operation.
 *
	tail = Q->Tail;
	tail->Ptr->Next = newPointer;
	newNode->Prev = tail;
	Q->Tail = newPointer;
	Q->Longth++;
*/

//	CAS enqueue operation.
	while(TRUE){
		tail = Q->Tail;


	}
}


