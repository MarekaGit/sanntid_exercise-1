// Compile with `gcc foo.c -Wall -std=gnu99 -lpthread`, or use the makefile
// The executable will be named `foo` if you use the makefile, or `a.out` if you use gcc directly

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

int i = 0;

// Note the return type: void*
void* incrementingThreadFunction(){
    // TODO: increment i 1_000_000 times
    for (int j = 1; j < 1000001; ++j){
        i = i + 1;
    }
    return NULL;
}

void* decrementingThreadFunction(){
    // TODO: decrement i 1_000_000 times
    for (int j = 1; j < 1000001; ++j){
        i = i - 1;
    }
    return NULL;
}


int main(){

    // TODO: 
    // start the two functions as their own threads using `pthread_create`
    // Hint: search the web! Maybe try "pthread_create example"?
    // TODO:
    // wait for the two threads to be done before printing the final result
    // Hint: Use `pthread_join`    

    pthread_t inc_thread;
    pthread_t dec_thread;
    
    int errInc = pthread_create(&inc_thread, NULL, incrementingThreadFunction, "Incrementing Thread");
    int errDec = pthread_create(&dec_thread, NULL, decrementingThreadFunction, "Decrementing Thread");
  
    if (errInc){
        printf("Thread creation failed :  %s\n", strerror(errInc));
        return errInc;
    }
    if (errDec){
        printf("Thread creation failed :  %s\n", strerror(errDec));
        return errDec;
    }
    else{
        printf("Inc thread Created with ID :  %ld\n", inc_thread);
        printf("Dec thread Created with ID :  %ld\n", dec_thread);
    }

    errInc = pthread_join(inc_thread, NULL);
    // check if joining is sucessful
    errDec = pthread_join(dec_thread, NULL);
    // check if joining is sucessful

    if (errInc)
    {
        printf("Failed to join Thread :  %s\n", strerror(errInc));
        return errInc;
    }
    if (errDec)
    {
        printf("Failed to join Thread :  %s\n", strerror(errDec));
        return errDec;
    }
    
    printf("The magic number is: %d\n", i);
    return 0;  
}
