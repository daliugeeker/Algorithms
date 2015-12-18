/*
 * NonBlockingConcurrentQueue.c
 *
 *  Created on: Dec 11, 2015
 *      Author: liuda
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stddef.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>

//#define TRUE 1
//#define FALSE 0

typedef int DataType;

typedef struct node_t{
	DataType Value;
	struct pointer_t * volatile Next; //The pointer stores the address of next Pointer.
}Node;

typedef struct pointer_t{
	unsigned int Tag;
	Node * volatile Ptr; //The pointer stores the address of the current node.
}Pointer;

typedef struct queue_t{
	Pointer * volatile Head; //The pointer stores the address of the head Pointer of the queue.
	Pointer * volatile Tail; //The pointer stores the address of the tail Pointer of the queue.
	int Longth;
}Queue;

Queue * initialize(Queue * Q){
	Pointer * volatile pointer= (Pointer*)malloc(sizeof(Pointer));
	Q = (Queue *)malloc(sizeof(Queue));
	pointer->Ptr->Value = 0;
	pointer->Tag = 0;
	pointer->Ptr->Next = NULL;

	Q->Head = Q->Tail = pointer;
	Q->Longth = 0;
	if(!Q->Head)
		exit(1);
	else
		return Q;
}

Pointer * volatile newPointer1;

void enqueue1(Queue *Q, DataType val){
	newPointer1 = (Pointer*)malloc(sizeof(Pointer));
//	Q = (Queue *)malloc(sizeof(Queue));
	if(newPointer1 == NULL){
		printf("ERROR!\n");
		exit(1);
	}
	if(Q == NULL){
		printf("ERROR!\n");
		exit(1);
	}

	newPointer1->Tag=0;
	newPointer1->Ptr->Value = val;
	newPointer1->Ptr->Next = NULL; //Set the new tail's next pointer field to be null.
	sleep(10); //Make the thread sleep for 10 seconds.
	Q->Tail->Ptr->Next = newPointer1; //Heritage of the Queue tail: the last Queue tail points to the newNode.
//	sleep(10); //Make the thread sleep for 10 seconds.
	Q->Tail = newPointer1; //The newNode becomes the new tail.
	Q->Longth++;
}


void enqueue2(Queue *Q, DataType val){
	Pointer * volatile newPointer2 = (Pointer*)malloc(sizeof(Pointer));
//	Q = (Queue *)malloc(sizeof(Queue));
	if(newPointer2 == NULL){
		printf("ERROR!\n");
		exit(1);
	}
	if(Q == NULL){
		printf("ERROR!\n");
		exit(1);
	}

	newPointer2->Tag=0;
	newPointer2->Ptr->Value = val;
	newPointer2->Ptr->Next = NULL; //Set the new tail's next pointer field to be null.

	Q->Tail->Ptr->Next = newPointer2; //Heritage of the Queue tail: the last Queue tail points to the newNode.
	Q->Tail = newPointer2; //The newNode becomes the new tail.
	Q->Longth++;
}
/*Employ multiple threads.*/

pthread_t thread[2];
Queue * Qu;

void * thread1(){
	Qu = initialize(Qu);
	printf("Enqueue Operation One.");
	while(Qu->Longth<10)
		enqueue1(Qu,2);
	return NULL;
}

void * thread2(){
	free(newPointer1);
	printf("Enqueue Operation Two.");
	while(Qu->Longth<10)
		enqueue2(Qu,3);
	return NULL;
}

int main(){

	memset(&thread, 0, sizeof(thread));
	int ret1, ret2;
	ret1 = pthread_create(&thread[0],NULL,thread1,NULL);
	ret2 = pthread_create(&thread[1],NULL,thread2,NULL);
	while(Qu->Longth<10)
		printf("%2d\n",Qu->Tail->Ptr->Value);
	return 0;
}
