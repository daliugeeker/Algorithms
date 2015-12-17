/*
 * Multi-thread Single Queue Operations.c
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

typedef struct pointer_t{
	unsigned int tag;
	struct pointer_t * volatile ptrnext;
}Pointer;

typedef struct node_t{
	DataType value;
	Pointer * volatile next;
}Node;

typedef struct queue_t{
	Pointer * volatile head;
	Pointer * volatile tail;
	int longth;
}Queue;

Queue * initialize(Queue * Q){

	Node * volatile node= (Node*)malloc(sizeof(Node));
	Q = (Queue *)malloc(sizeof(Queue));
	node->value = 0;
	node->next->tag =0;
	node->next->ptrnext=NULL;
	Q->head = Q->tail = node;
	Q->longth = 0;
	if(!Q->head)
		exit(1);
	else
		return Q;
}

Node* volatile newNode1;

void enqueue1(Queue *Q, DataType val){
	newNode1 = (Node*)malloc(sizeof(Node));
//	Q = (Queue *)malloc(sizeof(Queue));
	if(newNode1 == NULL){
		printf("ERROR!\n");
		exit(1);
	}
	if(Q == NULL){
		printf("ERROR!\n");
		exit(1);
	}
	newNode1->next->tag=0;
	Q->tail->ptrnext = newNode1; //Heritage of the Queue tail: the last Queue tail points to the newNode.
	Q->tail = newNode1; //The newNode becomes the new tail.
	sleep(10); //Make the thread sleep for 10 seconds.
	newNode1->value = val;
	newNode1->next->ptrnext = NULL; //Set the new tail's next pointer field to be null.
	Q->longth++;
}


void enqueue2(Queue *Q, DataType val){
	Node* volatile newNode2 = (Node*)malloc(sizeof(Node));
//	Q = (Queue *)malloc(sizeof(Queue));
	if(newNode2 == NULL){
		printf("ERROR!\n");
		exit(1);
	}
	if(Q == NULL){
		printf("ERROR!\n");
		exit(1);
	}
	newNode2->next->tag=0;
	newNode2->value = val;
	newNode2->next->tag=0;

	Q->tail->ptrnext = newNode2; //Heritage of the Queue tail: the last Queue tail points to the newNode.
	Q->tail = newNode2; //The newNode becomes the new tail.
	newNode2->next->ptrnext = NULL; //Set the new tail's next pointer field to be NULL.
	Q->longth++;
}
/*Employ multiple threads.*/

pthread_t thread[2];
Queue * Qu;
void * thread1(){
	Qu = initialize(Qu);
	printf("Enqueue Operation One.");
	while(Qu->longth<10)
		enqueue1(Qu,2);
	return NULL;
}

void * thread2(){
	free(newNode1);
	Node* volatile newNode2 = (Node*)malloc(sizeof(Node));
	printf("Enqueue Operation Two.");
	while(Qu->longth<10)
		enqueue2(Qu,3);
	return NULL;
}

int main(){
	memset(&thread, 0, sizeof(thread));
	int ret1, ret2;
	ret1 = pthread_create(&thread[0],NULL,thread1,NULL);
	ret2 = pthread_create(&thread[1],NULL,thread2,NULL);

	if(ret1!=0 && ret2==0)
		printf("Success!");
	return 0;
}
