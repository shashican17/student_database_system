#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <semaphore.h>

typedef struct {
    sem_t mutex;
   
} SharedMemory;

void init_shared_memory(SharedMemory* shm) {
    sem_init(&shm->mutex, 1, 1);  
    
}

#endif