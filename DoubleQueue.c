#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stddef.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#define TRUE 1
#define FALSE 0

typedef int DataType;

typedef struct pointer{
	uint64_t Tag;
	struct node * volatile Ptr;
}*Pointer;

typedef struct node{
	DataType Value;
	Pointer volatile Next;
	Pointer volatile Previous;
}Node;

typedef struct queue{
	Pointer volatile Tail;
	Pointer volatile Head;
	int Longth;
}Queue;

int32_t InitializeQueue(Queue * volatile Q){
	Pointer volatile pointer = (Pointer)malloc(sizeof(Pointer));
	Node * volatile node = (Node*)malloc(sizeof(Node));
	Q = (Queue*)malloc(sizeof(Queue));
	//Connect pointer and node.
	pointer->Tag = 0;
	pointer->Ptr = node;
	//Set node.
	node->Value = -100;
	node->Next = node->Previous = NULL;
	//Connect Q and pointer.
	Q->Longth = 0;
	Q->Head = Q->Tail = pointer;

	if(Q){
		printf("Initialize Queue Success!");
		return TRUE;
	}
	else
		exit(1);
}

void Enqueue(Queue * Q, DataType val, int SleepTime){
	if(Q == NULL)
		printf("No Queue Exists!");
	Pointer volatile newPointer = (Pointer)malloc(sizeof(Pointer));
	Node * volatile newNode = (Node*)malloc(sizeof(Node));

	newPointer->Tag = 0;
	newPointer->Ptr = newNode;

	newNode->Value = val;
	newNode->Next = newNode->Previous = NULL;

	newNode->Next = Q->Tail;
	Q->Tail->Ptr->Previous = newPointer;
	Q->Tail = newPointer;
	Q->Longth++;

	free(newNode);
	free(newPointer);
}
