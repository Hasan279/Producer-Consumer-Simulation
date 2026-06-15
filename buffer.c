#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "buffer.h"
#include "logger.h"

static pthread_mutex_t operation_mutex = PTHREAD_MUTEX_INITIALIZER;

LogMessage buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

pthread_mutex_t mutex;
sem_t empty_slots;
sem_t full_slots;

void init_buffer(void)
{
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        buffer[i].service_id = BUFFER_EMPTY;
    }

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty_slots, 0, BUFFER_SIZE);
    sem_init(&full_slots, 0, 0);
}

void produce_item(int producer_id)
{
    LogMessage item;
    item.service_id = producer_id;

    int rand_val = rand() % 100;
    if (rand_val < 75) {
        strcpy(item.log_level, "INFO");
        strcpy(item.message, "User login successful");
    } else if (rand_val < 95) {
        strcpy(item.log_level, "WARN");
        strcpy(item.message, "Database connection pool low");
    } else {
        strcpy(item.log_level, "ERROR");
        strcpy(item.message, "API request failed");
    }

    sem_wait(&empty_slots);
    if (!running) {
        sem_post(&empty_slots);
        return;
    }
    pthread_mutex_lock(&operation_mutex);
    pthread_mutex_lock(&mutex);

    buffer[in] = item;
    in = (in + 1) % BUFFER_SIZE;
    
    int full_count = 0;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (buffer[i].service_id != BUFFER_EMPTY) full_count++;
    }

    pthread_mutex_unlock(&mutex);
    sem_post(&full_slots);

    log_produced(item, full_count);
    sleep(1);
    pthread_mutex_unlock(&operation_mutex);
}

void consume_item(int consumer_id)
{
    sem_wait(&full_slots);
    if (!running) {
        sem_post(&full_slots);
        return;
    }
    pthread_mutex_lock(&operation_mutex);
    pthread_mutex_lock(&mutex);

    LogMessage item = buffer[out];
    buffer[out].service_id = BUFFER_EMPTY;
    out = (out + 1) % BUFFER_SIZE;

    int full_count = 0;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (buffer[i].service_id != BUFFER_EMPTY) full_count++;
    }

    pthread_mutex_unlock(&mutex);
    sem_post(&empty_slots);

    log_consumed(consumer_id, item, full_count);
    sleep(1);
    pthread_mutex_unlock(&operation_mutex);
}
