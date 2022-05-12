CC = gcc
CFLAGS = -Wall -g

all: run clean

prepare:
	$(CC) $(FLAGS) chef.c -o chef -lpthread -lrt
	$(CC) $(FLAGS) supplier.c -o supplier -lpthread -lrt
	$(CC) $(FLAGS) main.c -o main -lpthread -lrt

clean:
	rm -f chef
	rm -f main
	rm -f supplier

run:
	make clean
	make prepare
	./main 3 3