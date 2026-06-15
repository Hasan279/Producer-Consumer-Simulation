#ifndef BUFFER_H
#define BUFFER_H

#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5
#define BUFFER_EMPTY (-1)

typedef struct {
    int service_id;
    char log_level[8];
    char message[64];
} LogMessage;

extern volatile int running;

extern LogMessage buffer[BUFFER_SIZE];
extern int in;
extern int out;

extern pthread_mutex_t mutex;
extern sem_t empty_slots;
extern sem_t full_slots;

void init_buffer(void);
void produce_item(int producer_id);
void consume_item(int consumer_id);

#endif