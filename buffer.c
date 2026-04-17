#include <stdio.h>
#include "buffer.h"

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

pthread_mutex_t mutex;
sem_t empty_slots;
sem_t full_slots;

void init_buffer() {
    // fill buffer with -1 to show empty slots
    for (int i = 0; i < BUFFER_SIZE; i++)
        buffer[i] = -1;

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty_slots, 0, BUFFER_SIZE);  // all slots empty at start
    sem_init(&full_slots, 0, 0);             // no full slots at start
}

void insert_item(int item) {
    sem_wait(&empty_slots);       // wait if buffer is full
    pthread_mutex_lock(&mutex);

    buffer[in] = item;
    in = (in + 1) % BUFFER_SIZE; // circular buffer

    pthread_mutex_unlock(&mutex);
    sem_post(&full_slots);        // signal that a new item is ready
}

int remove_item() {
    sem_wait(&full_slots);         // wait if buffer is empty
    pthread_mutex_lock(&mutex);

    int item = buffer[out];
    buffer[out] = -1;
    out = (out + 1) % BUFFER_SIZE;

    pthread_mutex_unlock(&mutex);
    sem_post(&empty_slots);        // signal that a slot is now free

    return item;
}

void print_buffer() {
    printf("Buffer: [");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (buffer[i] == -1)
            printf(" -");
        else
            printf("%2d", buffer[i]);
        if (i < BUFFER_SIZE - 1) printf(" |");
    }
    printf(" ]\n");
}