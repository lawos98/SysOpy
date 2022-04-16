CC = gcc
CFLAGS = -Wall -Werror -g
FILENAME=raport.txt

run:
	make clean
	$(CC) $(CFLAGS)  main.c -o main -ldl
	./main >> $(FILENAME)

clean:
	@rm -rf *.o *.so *.a main

clean_raport:
	@rm -f $(FILENAME)


