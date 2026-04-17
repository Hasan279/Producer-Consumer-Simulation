#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "buffer.h"
#include "logger.h"

extern int running; // set to 0 when simulation ends

void *producer(void *arg) {
    int id = *(int *)arg;

    while (running) {
        int item = rand() % 100; // random item between 0-99
        insert_item(item);
        log_produced(id, item);
        print_buffer();

        sleep(rand() % 2 + 1); // sleep 1 or 2 seconds (variable rate)
    }

    return NULL;
}