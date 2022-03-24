CC = gcc
CFLAGS = -Wall -Werror -g
FILENAME=raport.txt

run:
	make clean
	$(CC) $(CFLAGS)  -fPIC -shared lib.c -o liblib.so
	$(CC) $(CFLAGS)  main.c -L. -Wl,-rpath=. -llib -o main
	./main in.txt out.txt >> $(FILENAME)

clean:
	@rm -rf *.o *.so *.a main

clean_raport:
	@rm -f $(FILENAME)


