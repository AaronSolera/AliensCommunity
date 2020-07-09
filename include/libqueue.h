#ifndef LIBQUEUE_H
#define LIBQUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Node
{
	void *data;
	struct Node *next;
};

struct Queue
{
	int size;
	int element_size;
	struct Node *front;
	struct Node *rear;
};

/* 
	Function for initializing the queue 
*/
int initQueue(struct Queue *queue, int element_size);

/* 
	Function for creating a Node 
*/
struct Node * createNode(void *element, int element_size);

/* 
	Function for checking whether the queue is empty
*/
int isEmpty(struct Queue *queue);

/* 
	Inserts an element at rear of queue
*/
int enqueue(struct Queue *queue, void *element);

/* 
	Removes an element from front of queue and gets its value
*/
int dequeue(struct Queue *queue, void *target);

/* 
	Deallocates the memory allocated to queue 
*/
int destroy(struct Queue *queue);

/*
	Function to sort a queue using Bubble Sort algorithm
*/
int bubbleSort(struct Queue *queue, int length, int (*compare)(const void *a, const void *b));

/*
	Function to swap data of two nodes a and b
*/
void swap(struct Node *a, struct Node *b);

#endif
