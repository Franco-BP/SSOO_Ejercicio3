#include <stdio.h>
#include <stdlib.h>
#include "genqueue.h"

// Generic Queue

// Function to initialize the queue
void initializeQueue(Queue* queue) {
    queue->front = queue->rear = NULL;
}

// Function to check if the queue is empty
int isQueueEmpty(Queue* queue) {
    return (queue->front == NULL);
}

// Function to enqueue an element
void enqueue(Queue* queue, void* data) {
    // Create a new node
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;

    // If the queue is empty, set the new node as both front and rear
    if (isQueueEmpty(queue)) {
        queue->front = queue->rear = newNode;
    } else {
        // Otherwise, add the new node at the end of the queue
        queue->rear->next = newNode;
        queue->rear = newNode;
    }

}

// Function to dequeue an element
void* dequeue(Queue* queue) {
    if (isQueueEmpty(queue)) {
        printf("Error: Queue is empty.\n");
        return NULL;
    }

    // Retrieve the data from the front node
    Node* frontNode = queue->front;
    void* data = frontNode->data;

    // Move the front pointer to the next node
    queue->front = frontNode->next;

    // If the queue becomes empty, update the rear pointer as well
    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    // Free the memory of the dequeued node
    free(frontNode);

    return data;
}

// Function to free the memory used by the queue
void freeQueue(Queue* queue) {
    while (!isQueueEmpty(queue)) {
        dequeue(queue);
    }
}


