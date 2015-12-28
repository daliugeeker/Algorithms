/*
 *  Multi-thread_Single_Queue_Operations.c
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

#define TRUE 1
//#define FALSE 0

typedef int DataType;

typedef struct node_t{
	DataType Value;
	struct pointer_t * volatile Next; //The pointer stores the address of next Pointer.
}Node;

typedef struct pointer_t{
	unsigned int Tag;
	struct node_t * volatile Ptr; //The pointer stores the address of the current node.
}Pointer;

typedef struct queue_t{
	Pointer * volatile Head; //The pointer stores the address of the head Pointer of the queue.
	Pointer * volatile Tail; //The pointer stores the address of the tail Pointer of the queue.
	int Longth;
}Queue;

Queue * initializeQueue(Queue * volatile Q){
	Pointer * volatile pointer= (Pointer*)malloc(sizeof(Pointer));
	Node * volatile node = (Node*)malloc(sizeof(Node));
	Q = (Queue *)malloc(sizeof(Queue));
	pointer->Ptr = node;
	pointer->Tag = 0;
	node->Value = -100;
	node->Next = NULL;

	Q->Head = Q->Tail = pointer;
//	Q->Head->Ptr = Q->Tail->Ptr = node;
//	Q->Head->Tag = Q->Tail->Tag = 0;
	Q->Longth = 0;
	if(!Q->Head)
		exit(1);
	else
		return Q;
}

Pointer * volatile newPointer1;
Node * volatile newNode1;
void enqueue1(Queue * volatile Q, DataType val, int sleeptime){
	newPointer1 = (Pointer*)malloc(sizeof(Pointer));
	newNode1 = (Node*)malloc(sizeof(Node));
	Pointer * tailPointer = (Pointer*)malloc(sizeof(Pointer));
	Node * tailNode = (Node*)malloc(sizeof(Node));
	Pointer * nextPointer = (Pointer*)malloc(sizeof(Pointer));
	if(newPointer1 == NULL){
		printf("ERROR!\n");
		exit(1);
	}
	if(newNode1 == NULL){
		printf("ERROR!\n");
		exit(1);
	}
	if(Q == NULL){
		printf("ERROR!\n");
		exit(1);
	}

	newPointer1->Tag=0;
	newPointer1->Ptr = newNode1;
	newNode1->Value = val;
	newNode1->Next = NULL; //Set the new tail's next pointer field to be null.

	sleep(sleeptime);

	while(TRUE){
		tailPointer = Q->Tail; //Read the current tail pointer of the queue.
		tailNode = tailPointer->Ptr;
		nextPointer = tailNode->Next;
		sleep(10); //Sleep for ten seconds.
		if(tailPointer == Q->Tail){
			if(tailNode->Next == NULL){
				if(__sync_bool_compare_and_swap(&(tailNode->Next),&nextPointer,&newPointer1) && (tailNode->Next->Tag == nextPointer->Tag)){
					newPointer1->Tag++;
					break;
				}
			}
			else{
				__sync_bool_compare_and_swap(&(Q->Tail),&tailPointer,&nextPointer);
				tailPointer->Tag++;
			}
		}
		__sync_bool_compare_and_swap(&Q->Tail,tailPointer,newPointer1);
		tailPointer->Tag++;
		Q->Longth++;
	}
	printf("%2d\n",newPointer1->Tag, nextPointer->Tag,tailPointer->Tag);
/*
 * The standard enqueue operation.
 */
//	sleep(10); //Make the thread sleep for 10 seconds.
//	Q->Tail->Ptr->Next = newPointer1; //Heritage of the Queue tail: the last Queue tail points to the newNode.
//  sleep(10); //Make the thread sleep for 10 seconds.
//	Q->Tail = newPointer1; //The newNode becomes the new tail.
//	Q->Longth++;

	free(nextPointer);
	free(newNode1);
	free(newPointer1);
}


//void enqueue2(Queue * volatile Q, DataType val){
//	Pointer * volatile newPointer2 = (Pointer*)malloc(sizeof(Pointer));
//	Node * volatile newNode2 = (Node*)malloc(sizeof(Node));
////	Q = (Queue *)malloc(sizeof(Queue));
//	if(newPointer2 == NULL){
//		printf("ERROR!\n");
//		exit(1);
//	}
//	if(newNode2 == NULL){
//			printf("ERROR!\n");
//			exit(1);
//	}
//	if(Q == NULL){
//		printf("ERROR!\n");
//		exit(1);
//	}
//
//	newPointer2->Tag=0;
//	newPointer2->Ptr = newNode2;
//	newNode2->Value = val;
//	newNode2->Next = NULL; //Set the new tail's next pointer field to be null.
//
//	Q->Tail->Ptr->Next = newPointer2; //Heritage of the Queue tail: the last Queue tail points to the newNode.
//	Q->Tail = newPointer2; //The newNode becomes the new tail.
//	Q->Longth++;
//
//	free(newPointer2);
//	free(newNode2);
//}
/*Employ multiple threads.*/

pthread_t thread[2];
Queue * Qu;

void * thread1(){
//	Qu = initializeQueue();
	printf("Enqueue Operation One.\n");
//	while(Qu->Longth<10)
//		enqueue1(Qu,2);
	enqueue1(Qu,17,10);
	return NULL;
}

void * thread2(){
	free(newNode1);
	free(newPointer1);
	printf("Enqueue Operation Two.\n");
//	while(Qu->Longth<10)
//		enqueue2(Qu,3);
	enqueue1(Qu,13,0);
	return NULL;
}

int main(){
	Qu = initializeQueue(Qu);
//	memset(&thread, 0, sizeof(thread));
	int ret1, ret2;
	ret1 = pthread_create(&thread[0],NULL,thread1,NULL);
	ret2 = pthread_create(&thread[1],NULL,thread2,NULL);
	printf("Thread one runs:%d\n",ret1);
	printf("Thread two runs:%d\n",ret2);
//	while(Qu->Longth<10)
//		printf("%2d\n",Qu->Tail->Ptr->Value);
	printf("%2d\n",Qu->Tail->Ptr->Value);

	free(Qu);
	return 0;
}
