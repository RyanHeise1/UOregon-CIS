/*
 * implementation for linked-list-based generic FIFO queue
 */

#include "ADTs/llistqueue.h"
#include <stdlib.h>

typedef struct q_node {
    struct q_node *next;
    void *value;
} QNode;

typedef struct q_data {
    long size;
    QNode *head;
    QNode *tail;
    void (*freeValue)(void *e);
} QData;

static void purge(QData *qd){
    // same logic on page 249
    if(qd->freeValue != NULL){
        QNode *n;
        for(n = qd->head; n != NULL; n = n->next)
            qd->freeValue(n->value);
    }
}

static void clearList(QData *qd){
    QNode *p;
    QNode *q;
    for (p= qd->head; p != NULL; p = q){
        q = p->next;
        free(p);
    }
}

static void q_destroy(const Queue *q) {
    // same logic on page 249
    QData *qd = (QData *)q->self;
    purge(qd);
    clearList(qd);
    free(qd);
    free((void *)q);
}

static void q_clear(const Queue *q) {
    // same logic on page 249-250
    QData *qd = (QData *)q->self;
    clearList(qd);
    purge(qd);
    qd->size = 0L;
    qd->head = NULL;
    qd->tail = NULL;
}

static int q_enqueue(const Queue *q, void *element) {
    QData *qd = (QData *)q->self;
    QNode *new = (QNode *)malloc(sizeof(QNode));
    int status = (new != NULL);
    if (status){
        new->value = element;
        new->next = NULL;
        if (qd->head == NULL)
            qd->head = new;
        else
            qd->tail->next = new;
        qd->tail = new;
        qd->size++;
    }
    return status;
}

static int q_dequeue(const Queue *q, void **element) {
    QData *qd = (QData *)q->self;
    int status = (qd->size > 0L);
    if(status) {
        QNode *n = qd->head;
        if(n != NULL) {
            *element = n->value;
            qd->head = n->next;
            qd->size--;
            free(n);
            //qd->size--;
        }
    }
    return status;
}

static int q_front(const Queue *q, void **element) {
    QData *qd = (QData *)q->self;
    int status = (qd->size > 0L);
    if (status)
        *element = qd->head->value;
    return status;
}

static long q_size(const Queue *q) {
    // same logic on page 251
    QData *qd = (QData *)q->self;
    return qd->size;
}

static int q_isEmpty(const Queue *q) {
    // same logic on page 251
    QData *qd = (QData *)q->self;
    return (qd->head == NULL);
}

static void **toArray(QData *qd){
    // same logic on page 251
    void **array = NULL;
    if (qd->size > 0L){
        array = (void **)malloc(qd->size * sizeof(void *));
        if (array != NULL){ // unable to allocate mem
            QNode *n;
            long i = 0L;
            for (n = qd->head; n != NULL; n = n->next)
                array[i++] = n->value;
        }
    }
    return array;
}

static void **q_toArray(const Queue *q, long *len) {
    // same logic on page 251
    QData *qd = (QData *)q->self;
    void **tmp = toArray(qd);

    if(tmp != NULL)
        *len = qd->size;
    return tmp;
}

static const Iterator *q_itCreate(const Queue *q) {
    // logic taken from page 251
    QData *qd = (QData *)q->self;
    const Iterator *it = NULL;
    void **tmp = toArray(qd);
    if (tmp != NULL){
        it = Iterator_create(qd->size, tmp);
        if (it != NULL)
            free(tmp);
    }
    return it;
}

static const Queue *q_create(const Queue *q);
/* this is just declaring the signature for the create() method; it's
   implementation is provided below */

static Queue template = {
    NULL, q_create, q_destroy, q_clear, q_enqueue, q_front, q_dequeue, q_size,
    q_isEmpty, q_toArray, q_itCreate
};

static const Queue *createQueue(void (*freeValue)(void *e)){
    // logic taken from pg 252 of the book
    Queue *q = (Queue *)malloc(sizeof(Queue));
    if (q != NULL){
        QData *qd= (QData *)malloc(sizeof(QData));
        if (qd != NULL){
            qd->size = 0L;
            qd->head = NULL;
            qd->tail = NULL;
            *q = template;
            q->self = qd;
            qd->freeValue = freeValue;
        }else{
            free(q);
            q = NULL;
        }
    } 
    return q;
}

static const Queue *q_create(const Queue *q) {
    QData *qd = (QData *)q->self;
    return createQueue(qd->freeValue);
}

const Queue *LListQueue(void (*freeValue)(void *e)) {
    return createQueue(freeValue);
}

const Queue *Queue_create(void (*freeValue)(void *e)) {
    return createQueue(freeValue);
}
