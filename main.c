#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "buffer.h"
#include "Producer.h"
#include "Consumer.h"
#include "logger.h"

#define NUM_PRODUCERS 3
#define NUM_CONSUMERS 3
#define SIM_DURATION  15  // simulation runs for 15 seconds

int running = 1; // threads check this to know when to stop

int main() {
    printf("=== Producer-Consumer Simulation ===\n\n");

    init_buffer();

    // create thread arrays
    pthread_t prod_threads[NUM_PRODUCERS];
    pthread_t cons_threads[NUM_CONSUMERS];

    // ids so each thread knows its number
    int prod_ids[NUM_PRODUCERS];
    int cons_ids[NUM_CONSUMERS];

    // start producer threads
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        prod_ids[i] = i + 1;
        pthread_create(&prod_threads[i], NULL, producer, &prod_ids[i]);
    }

    // start consumer threads
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        cons_ids[i] = i + 1;
        pthread_create(&cons_threads[i], NULL, consumer, &cons_ids[i]);
    }

    // let simulation run
    sleep(SIM_DURATION);

    // tell all threads to stop
    running = 0;

    // wake up any threads stuck waiting on semaphores so they can exit
    for (int i = 0; i < NUM_PRODUCERS + BUFFER_SIZE; i++) sem_post(&empty_slots);
    for (int i = 0; i < NUM_CONSUMERS + BUFFER_SIZE; i++) sem_post(&full_slots);

    // wait for all threads to finish
    for (int i = 0; i < NUM_PRODUCERS; i++) pthread_join(prod_threads[i], NULL);
    for (int i = 0; i < NUM_CONSUMERS; i++) pthread_join(cons_threads[i], NULL);

    print_stats();

    // cleanup
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty_slots);
    sem_destroy(&full_slots);

    return 0;
}