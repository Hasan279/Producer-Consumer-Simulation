CC = gcc
CFLAGS = -Wall
SRCS = main.c buffer.c Producer.c Consumer.c logger.c
TARGET = simulation

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) -lpthread

clean:
	rm -f $(TARGET)