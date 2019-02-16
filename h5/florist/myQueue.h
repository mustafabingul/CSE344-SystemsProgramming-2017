#include <stdlib.h>
#include <stdio.h>

#define TRUE  1
#define FALSE	0

typedef struct location{
    int x;
    int y;
}loca;
typedef struct {
    char Cname[15];
    struct location Clocation;
    char Cflower[15];
} DATA;
typedef struct Node_t {
    DATA data;
    struct Node_t *prev;
} NODE;
typedef struct Queue {
    NODE *head;
    NODE *tail;
    int size;
    int limit;
} Queue;

Queue *concreateQue(int limit);
void destroyQue(Queue *queue);
int enq(Queue *pQueue, NODE *item);
NODE *deq(Queue *pQueue);
int emptyQ(Queue *pQueue);

Queue *concreateQue(int limit) {
    Queue *queue = (Queue*) malloc(sizeof (Queue));
    if (queue == NULL) {
        return NULL;
    }
    if (limit <= 0) {
        limit = 65535;
    }
    queue->limit = limit;
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}

void destroyQue(Queue *queue) {
    NODE *pN;
    while (!emptyQ(queue)) {
        pN = deq(queue);
        free(pN);
    }
    free(queue);
}

int enq(Queue *pQueue, NODE *item) {
    if ((pQueue == NULL) || (item == NULL)) {
        return FALSE;
    }
    if (pQueue->size >= pQueue->limit) {
        return FALSE;
    }
    item->prev = NULL;
    if (pQueue->size == 0) {
        pQueue->head = item;
        pQueue->tail = item;

    } else {
        pQueue->tail->prev = item;
        pQueue->tail = item;
    }
    pQueue->size++;
    return TRUE;
}

NODE * deq(Queue *pQueue) {
    NODE *item;
    if (emptyQ(pQueue))
    {
        printf("NYLLLLLLLLLLLQQQQQQQQQQQQQQQQ");
        return NULL;
    }
    item = pQueue->head;
    pQueue->head = (pQueue->head)->prev;
    pQueue->size--;
    return item;
}

int emptyQ(Queue *pQueue) {
    if (pQueue == NULL) {
        return FALSE;
    }
    if (pQueue->size == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}