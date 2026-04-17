#ifndef BUFFER_H
#define BUFFER_H

#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5

// the shared buffer and its index
extern int buffer[BUFFER_SIZE];
extern int in;   // producer puts item here
extern int out;  // consumer takes item from here

// mutex and semaphores
extern pthread_mutex_t mutex;
extern sem_t empty_slots;  // how many empty slots are left
extern sem_t full_slots;   // how many full slots are ready

void init_buffer();
void insert_item(int item);
int remove_item();
void print_buffer();

#endif