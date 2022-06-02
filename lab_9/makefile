CC = gcc
CFLAGS = -Wall -g

all: run clean

compile:
	$(CC) $(FLAGS) main.c -o main -pthread

run:
	make compile
	./main

clean:
	rm -f main