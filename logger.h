#ifndef LOGGER_H
#define LOGGER_H

#include "buffer.h"

// stats tracked across all threads
extern int total_produced;
extern int total_consumed;

void log_produced(LogMessage item, int full_slots_count);
void log_consumed(int indexer_id, LogMessage item, int full_slots_count);
void print_stats();

#endif