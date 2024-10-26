#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "utils.h"

// Initialize an empty queue with front and rear pointers set to NULL
void initialize_queue(queue_t* q) {
    q->front = NULL;
    q->rear = NULL;
}

// Enqueue operation to add a new node at the end of the queue
void enqueue(queue_t* q, node_t* n) {
    queue_node_t* new_node = (queue_node_t*)malloc(sizeof(queue_node_t));
    if (!new_node) {
        printf("Error: Memory allocation failed\n");
        return;
    }
    new_node->data = n;
    new_node->next = NULL;

    if (is_queue_empty(q)) {
        q->front = new_node;
        q->rear = new_node;
    } else {
        q->rear->next = new_node;
        q->rear = new_node;
    }
}

// Dequeue operation to remove the node at the front of the queue
void dequeue(queue_t* q) {
    if (is_queue_empty(q)) {
        printf("Error: No element to dequeue\n");
        return;
    }
    queue_node_t* temp = q->front;
    q->front = q->front->next;

    if (q->front == NULL) {  // Queue becomes empty after dequeue
        q->rear = NULL;
    }
    free(temp);
}

// Dequeue and return the node at the front of the queue
node_t* dequeue_node(queue_t* q) {
    if (is_queue_empty(q)) {
        printf("Error: No element to dequeue\n");
        return NULL;
    }
    node_t* front_data = q->front->data;
    dequeue(q);
    return front_data;
}

// Check if the queue is empty
int is_queue_empty(queue_t* q) {
    return (q->front == NULL);
}

// Print the queue by traversing from front to rear
void print_queue(queue_t* q) {
    printf("Queue: ");
    queue_node_t* current = q->front;
    while (current != NULL) {
        // drawBoard(&(current->data->state));
        current = current->next;
    }
    printf("\n");
}

// Free all nodes in the queue
void free_queue(queue_t* q) {
    while (!is_queue_empty(q)) {
        dequeue(q);
    }
    printf("Queue freed\n");
}
