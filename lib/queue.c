#include "../include/queue.h"

int initQueue(struct Queue *queue, int element_size)
{
	queue->size = 0;
	queue->element_size = element_size;
	queue->front = NULL;
	queue->rear = NULL;

	return 0;
}


struct Node * createNode(void *element, int element_size)
{
	struct Node *new_node = (struct Node *)calloc(1, sizeof(struct Node));

	if (new_node != NULL)
	{
		new_node->data = calloc(1, element_size);
		new_node->next = NULL;
		memcpy(new_node->data, element, element_size);
	}

	return new_node;
}


int isEmpty(struct Queue *queue)
{
	return queue->front == NULL;
}


int enqueue(struct Queue *queue, void *element)
{
	struct Node *new_node = createNode(element, queue->element_size);

	if (new_node == NULL)
		return -1;

	if (!isEmpty(queue))
	{
		queue->rear->next = new_node;
		queue->rear = new_node;
	}
	else
		queue->front = queue->rear = new_node;

	queue->size++;

	return 0;
}


int dequeue(struct Queue *queue, void *target)
{

	if (isEmpty(queue))
		return -1;

	memcpy(target, queue->front->data, queue->element_size);

	struct Node *tmp = queue->front;
	queue->front = queue->front->next;
	queue->size--;

	free(tmp->data);
	free(tmp);

	return 0;
}


int destroy(struct Queue *queue)
{
	struct Node *current_node;

	while (queue->front != NULL)
	{
		current_node = queue->front;

		queue->front = current_node->next;

		free(current_node->data);
		free(current_node);
	}

	return 0;
}


int bubbleSort(struct Queue *queue, int length, int (*compare)(const void *a, const void *b))
{
	if (isEmpty(queue))
		return -1;

	if (length > queue->size)
		length = queue->size;

	int swapped;

	for (int i = 0; i < length; i++)
	{
		struct Node *tmp = queue->front;
		swapped = 0;

		for (int j = 0; j < length - 1 - i; j++)
		{
			struct Node *n = tmp->next;
			
			if (compare(tmp->data, n->data))
			{
				swap(tmp, n);
				swapped = 1;
			}

			tmp = tmp->next;
		}

		if (swapped == 0)
			break;
	}

	return 0;
}


void swap(struct Node *a, struct Node *b)
{
	void *tmp = a->data;
	a->data = b->data;
	b->data = tmp;
}
