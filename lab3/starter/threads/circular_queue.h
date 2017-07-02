#include <stdio.h>
#include <stdlib.h>

// Data structure for queue
typedef struct queue
{
	int *items;		// array to store queue elements
	int maxsize;	// maximum capacity of the queue
	int front;		// front points to front element in the queue (if any)
	int rear;		// rear points to last element in the queue
	int size;		// current capacity of the queue
} circular_queue;

// Utility function to initialize queue
int newQueue(struct queue *q, int size)
{
	// struct queue *q = NULL;
	// q = (struct queue*)malloc(sizeof(struct queue));
	if (q == NULL) {return -1;}
	q->items = (int*)malloc(size * sizeof(int));
	q->maxsize = size;
	q->front = 0, q->rear = -1;
	q->size = 0;
	return 0;
}

void destroy(struct queue *q){
	free(q->items);
	return;
}
// Utility function to return the size of the queue
int size(struct queue *pt)
{
	return pt->size;
}

// Utility function to check if the queue is empty or not
int isEmpty(struct queue *pt)
{
	return !size(pt);
}

// Utility function to return front element in queue
int front(struct queue *pt)
{
	if (isEmpty(pt))
	{
		// printf("UnderFlow\nProgram Terminated\n");
		exit(EXIT_FAILURE);
	}

	return pt->items[pt->front];
}

// Utility function to add an element x in the queue
void enqueue(struct queue *pt, int x)
{
	if (size(pt) == pt->maxsize)
	{
		// printf("OverFlow\nProgram Terminated\n");
		exit(EXIT_FAILURE);
	}

	// printf("Inserting %d\t", x);

	pt->rear = (pt->rear + 1) % pt->maxsize;	// circular queue
	pt->items[pt->rear] = x;
	pt->size++;

	// printf("front = %d, rear = %d\n", pt->front, pt->rear);
}

// Utility function to remove element from the queue
void dequeue(struct queue *pt, int* value)
{
	if (isEmpty(pt)) // front == rear
	{
		// printf("UnderFlow\nProgram Terminated\n");
		exit(EXIT_FAILURE);
	}

	*value = front(pt);
	//printf("Removing  %d\t", front(pt));

	pt->front = (pt->front + 1) % pt->maxsize;	// circular queue
	pt->size--;

	// printf("front = %d, rear = %d\n", pt->front, pt->rear);
}
