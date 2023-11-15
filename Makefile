CC=gcc
CFLAGS=-ansi -g

all: cs240StreamingService

cs240StreamingService: main.o streaming_service.o
	$(CC) $(CFLAGS) -o cs240StreamingService main.o streaming_service.o

main.o: main.c streaming_service.h
	$(CC) $(CFLAGS) -c main.c

streaming_service.o: streaming_service.c streaming_service.h
	$(CC) $(CFLAGS) -c streaming_service.c

clean:
	rm -f *.o cs240StreamingService

