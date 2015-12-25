/*
 *  Multi-thread_Single_Queue_Operations.c
 *
 *  Created on: Dec 11, 2015
 *      Author: liuda
 */

/* Pseudo-codes:
 *
  structure pointer_t {ptr: pointer to node_t, count: unsigned integer}
  structure node_t {value: data type, next: pointer_t}
  structure queue_t {Head: pointer_t, Tail: pointer_t}

  initialize(Q: pointer to queue_t)
     node = new_node()		// Allocate a free node
     node->next.ptr = NULL	// Make it the only node in the linked list
     Q->Head.ptr = Q->Tail.ptr = node	// Both Head and Tail point to it

  enqueue(Q: pointer to queue_t, value: data type)
   E1:   node = new_node()	// Allocate a new node from the free list
   E2:   node->value = value	// Copy enqueued value into node
   E3:   node->next.ptr = NULL	// Set next pointer of node to NULL
   E4:   loop			// Keep trying until Enqueue is done
   E5:      tail = Q->Tail	// Read Tail.ptr and Tail.count together
   E6:      next = tail.ptr->next	// Read next ptr and count fields together
   E7:      if tail == Q->Tail	// Are tail and next consistent?
               // Was Tail pointing to the last node?
   E8:         if next.ptr == NULL
                  // Try to link node at the end of the linked list
   E9:            if CAS(&tail.ptr->next, next, <node, next.count+1>)
  E10:               break	// Enqueue is done.  Exit loop
  E11:            endif
  E12:         else		// Tail was not pointing to the last node
                  // Try to swing Tail to the next node
  E13:            CAS(&Q->Tail, tail, <next.ptr, tail.count+1>)
  E14:         endif
  E15:      endif
  E16:   endloop
         // Enqueue is done.  Try to swing Tail to the inserted node
  E17:   CAS(&Q->Tail, tail, <node, tail.count+1>)

  dequeue(Q: pointer to queue_t, pvalue: pointer to data type): boolean
   D1:   loop			     // Keep trying until Dequeue is done
   D2:      head = Q->Head	     // Read Head
   D3:      tail = Q->Tail	     // Read Tail
   D4:      next = head.ptr->next    // Read Head.ptr->next
   D5:      if head == Q->Head	     // Are head, tail, and next consistent?
   D6:         if head.ptr == tail.ptr // Is queue empty or Tail falling behind?
   D7:            if next.ptr == NULL  // Is queue empty?
   D8:               return FALSE      // Queue is empty, couldn't dequeue
   D9:            endif
                  // Tail is falling behind.  Try to advance it
  D10:            CAS(&Q->Tail, tail, <next.ptr, tail.count+1>)
  D11:         else		     // No need to deal with Tail
                  // Read value before CAS
                  // Otherwise, another dequeue might free the next node
  D12:            *pvalue = next.ptr->value
                  // Try to swing Head to the next node
  D13:            if CAS(&Q->Head, head, <next.ptr, head.count+1>)
  D14:               break             // Dequeue is done.  Exit loop
  D15:            endif
  D16:         endif
  D17:      endif
  D18:   endloop
  D19:   free(head.ptr)		     // It is safe now to free the old node
  D20:   return TRUE                   // Queue was not empty, dequeue succeeded
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

Queue * initializeQueue(){
	Pointer * volatile pointer= (Pointer*)malloc(sizeof(Pointer));
	Node * volatile node = (Node*)malloc(sizeof(Node));
	Queue * volatile Q = (Queue *)malloc(sizeof(Queue));
	pointer->Ptr = node;
	pointer->Tag = 0;
	node->Value = -1;
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
void enqueue1(Queue * volatile Q, DataType val){
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

	while(TRUE){
		tailPointer = Q->Tail; //Read the current tail pointer of the queue.
		tailNode = tailPointer->Ptr;
		nextPointer = tailNode->Next;
//		sleep(10); //Sleep for ten seconds.
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

void enqueue2(Queue * volatile Q, DataType val){
	Pointer * volatile newPointer2 = (Pointer*)malloc(sizeof(Pointer));
	Node * volatile newNode2 = (Node*)malloc(sizeof(Node));
//	Q = (Queue *)malloc(sizeof(Queue));
	if(newPointer2 == NULL){
		printf("ERROR!\n");
		exit(1);
	}
	if(newNode2 == NULL){
			printf("ERROR!\n");
			exit(1);
	}
	if(Q == NULL){
		printf("ERROR!\n");
		exit(1);
	}

	newPointer2->Tag=0;
	newPointer2->Ptr = newNode2;
	newNode2->Value = val;
	newNode2->Next = NULL; //Set the new tail's next pointer field to be null.

	Q->Tail->Ptr->Next = newPointer2; //Heritage of the Queue tail: the last Queue tail points to the newNode.
	Q->Tail = newPointer2; //The newNode becomes the new tail.
	Q->Longth++;

	free(newPointer2);
	free(newNode2);
}
/*Employ multiple threads.*/

pthread_t thread[2];
Queue * Qu;

void * thread1(){
	Qu = initializeQueue();
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

//	free();

	return 0;
}
