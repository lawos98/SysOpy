CC = gcc
CFLAGS = -Wall -Werror -g
FILENAME=raport.txt
FILENAME2=raport2.txt

run:
	make clean
	$(CC) $(CFLAGS)  catcher.c -o catcher
	$(CC) $(CFLAGS)  sender.c -o sender

clean:
	@rm -rf *.o *.so *.a main

clean_raport:
	@rm -f $(FILENAME)


