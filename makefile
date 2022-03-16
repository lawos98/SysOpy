CC=gcc
CFLAGS=-Wall -g
DIR := ${CURDIR}

all:
	make static
	make shared

static:
	$(CC) $(CFLAGS) -c lib.c -o lib.o
	ar rcs lib.a lib.o
shared:
	$(CC) $(CFLAGS) -fPIC -c lib.c -o lib.o
	$(CC) $(CFLAGS) -shared -o lib.so lib.o
clean:
	@rm -rf *.o *.so *.a main

test_static: clean static testReports

testReports:
	$(CC) -c main.c
	$(CC) main.o lib.a -o main

	@echo "test 1: \n" >> raport2.txt
	./main `cat script/script1.txt`

	@echo "test 2: \n" >> raport2.txt
	./main `cat script/script2.txt`

	@echo "test 3: \n" >> raport2.txt
	./main `cat script/script3.txt`

	@echo "test 4: \n" >> raport2.txt
	./main `cat script/script4.txt`