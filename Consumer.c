#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "buffer.h"
#include "logger.h"

extern int running;

void *consumer(void *arg) {
    int id = *(int *)arg;

    while (running) {
        int item = remove_item();
        log_consumed(id, item);
        print_buffer();

        sleep(rand() % 3 + 1); // sleep 1-3 seconds (slightly slower than producer)
    }

    return NULL;
}