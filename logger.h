#ifndef LOGGER_H
#define LOGGER_H

// stats tracked across all threads
extern int total_produced;
extern int total_consumed;

void log_produced(int producer_id, int item);
void log_consumed(int consumer_id, int item);
void print_stats();

#endif