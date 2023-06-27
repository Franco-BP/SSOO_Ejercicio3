// Generic Queue

// Define the structure for the queue node
typedef struct Node {
    void* data;
    struct Node* next;
} Node;

// Define the structure for the queue
typedef struct {
    Node* front;
    Node* rear;
} Queue;

// Function to initialize the queue
void initializeQueue(Queue* queue);
// Function to check if the queue is empty
int isQueueEmpty(Queue* queue);
// Function to enqueue an element
void enqueue(Queue* queue, void* data);
// Function to dequeue an element
void* dequeue(Queue* queue);
// Function to free the memory used by the queue
void freeQueue(Queue* queue);