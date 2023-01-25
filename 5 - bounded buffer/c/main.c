// compile with:  gcc -g main.c ringbuf.c -lpthread

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#include "ringbuf.h"

struct BoundedBuffer {
    struct RingBuffer*  buf;
    pthread_mutex_t     mtx;
    sem_t               numElements2;
    sem_t               capacity2;
    int               numElements;
    int               capacity;
};

struct BoundedBuffer* buf_new(int size){
    struct BoundedBuffer* buf = malloc(sizeof(struct BoundedBuffer));
    buf->buf = rb_new(size);
    
    pthread_mutex_init(&buf->mtx, NULL);
    // TODO: initialize semaphores
    sem_init(&buf->capacity, 0, size);
	sem_init(&buf->numElements, 0, 0);
    buf->capacity = size;
    buf->numElements = 0;


/*
shared
(Input) An indication to the system of how the semaphore is going to be used.
A value of zero indicates that the semaphore will be used only by threads
within the current process. A nonzero value indicates that the semaphore may
be used by threads from other processes.

value
(Input) The value used to initialize the value of the semaphore.
*/
    
    return buf;    
}

void buf_destroy(struct BoundedBuffer* buf){
    rb_destroy(buf->buf);
    pthread_mutex_destroy(&buf->mtx);
    sem_destroy(&buf->numElements);
    sem_destroy(&buf->capacity);
    free(buf);
}




void buf_push(struct BoundedBuffer* buf, int val){    
    // TODO: wait for there to be room in the buffer
    // TODO: make sure there is no concurrent access to the buffer internals
    
    
    while(buf->numElements == buf->capacity)
    {
        struct timespec remaining, request = { 1, 10 };
        nanosleep(&request, &remaining);
        //printf("buf size: %d\n", buf->numElements); 
        //printf("buf cap: %d\n", buf->capacity); 
        /* code */
    }
    pthread_mutex_lock(&buf->mtx);
    rb_push(buf->buf, val);
    pthread_mutex_unlock(&buf->mtx);
    
    
    // TODO: signal that there are new elements in the buffer  
    buf->numElements++;
    sem_post(&buf->numElements2);
}

int buf_pop(struct BoundedBuffer* buf){
    // TODO: same, but different?

    while(buf->numElements < 1)
    {
        struct timespec remaining, request = { 5, 100 };
        nanosleep(&request, &remaining);
    }
    
    pthread_mutex_lock(&buf->mtx);
    int val = rb_pop(buf->buf); 
    pthread_mutex_unlock(&buf->mtx);  
    buf->numElements--;
    return val;
}





void* producer(void* args){
    struct BoundedBuffer* buf = (struct BoundedBuffer*)(args);
    printf("buf cap: %d\n", buf->capacity); 

    for(int i = 0; i < 10; i++){
        nanosleep(&(struct timespec){0, 100*1000*1000}, NULL);
        printf("[producer]: pushing %d\n", i);
        //printf("numElements buffer: %d\n", buf->numElements);
        //printf("numElements buffer: %d\n", &buf->numElements2);
        buf_push(buf, i);
    }
    return NULL;
}

void* consumer(void* args){
    struct BoundedBuffer* buf = (struct BoundedBuffer*)(args);
    
    // give the producer a 1-second head start
    nanosleep(&(struct timespec){1, 0}, NULL);
    while(1){
        int val = buf_pop(buf);
        printf("[consumer]: %d\n", val);
        nanosleep(&(struct timespec){0, 50*1000*1000}, NULL);
    }
}

int main(){ 
    
    struct BoundedBuffer* buf = buf_new(5);
    
    pthread_t producer_thr;
    pthread_t consumer_thr;
    pthread_create(&producer_thr, NULL, producer, buf);
    pthread_create(&consumer_thr, NULL, consumer, buf);
    
    pthread_join(producer_thr, NULL);
    pthread_cancel(consumer_thr);
    
    buf_destroy(buf);
    
    return 0;
}
