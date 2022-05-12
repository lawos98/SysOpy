CC = gcc
CFLAGS = -Wall -Werror -g

prepare:
	$(CC) $(FLAGS) supplier.c -o supplier
	$(CC) $(FLAGS) chef.c -o chef
	$(CC) $(FLAGS) main.c -o main

clean:
	rm -f chef
	rm -f main
	rm -f supplier

run:
	make clean
	make prepare
	./main 3 3