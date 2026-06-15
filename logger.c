#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include "logger.h"
#include "buffer.h"

int total_produced = 0;
int total_consumed = 0;

static pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

static double get_time() {
    static time_t start = 0;
    if (start == 0)
    {
        start = time(NULL);
    }
    return difftime(time(NULL), start);
}

void log_produced(LogMessage item, int full_slots_count) {
    pthread_mutex_lock(&print_mutex);
    total_produced++;
    printf("[%.1fs] Service-%d generated [%s]: %s | Buffer: [%d/%d slots full]\n", 
            get_time(), item.service_id, item.log_level, item.message, full_slots_count, BUFFER_SIZE);
    pthread_mutex_unlock(&print_mutex);
}

void log_consumed(int indexer_id, LogMessage item, int full_slots_count) {
    pthread_mutex_lock(&print_mutex);
    total_consumed++;
    printf("[%.1fs] Indexer-%d processed [%s]: %s | Buffer: [%d/%d slots full]\n", 
            get_time(), indexer_id, item.log_level, item.message, full_slots_count, BUFFER_SIZE);
    pthread_mutex_unlock(&print_mutex);
}

void print_stats() {
    pthread_mutex_lock(&print_mutex);
    printf("\n==========================================\n");
    printf("     Log Ingestion Pipeline Complete\n");
    printf("==========================================\n");
    printf(" Total Logs Generated : %d\n", total_produced);
    printf(" Total Logs Indexed   : %d\n", total_consumed);
    printf("==========================================\n");
    pthread_mutex_unlock(&print_mutex);
}