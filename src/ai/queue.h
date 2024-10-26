#ifndef __QUEUE__
#define __QUEUE__

#include "utils.h"
#include "node.h"

typedef struct queue_node {
    node_t* data;
    struct queue_node* next;
} queue_node_t;

typedef struct {
    queue_node_t* front;
    queue_node_t* rear;
} queue_t;

// Function declarations
void initialize_queue(queue_t* q);

void enqueue(queue_t* q, node_t* n);

void dequeue(queue_t* q);

node_t* dequeue_node(queue_t* q);

int is_queue_empty(queue_t* q);

void print_queue(queue_t* q);

void free_queue(queue_t* q);
#endif