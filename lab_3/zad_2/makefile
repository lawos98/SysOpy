CC = gcc
CFLAGS = -Wall -Werror -g
FILENAME=raport.txt

run:
	make clean
	$(CC) $(CFLAGS)  -fPIC -shared lib.c -o liblib.so
	$(CC) $(CFLAGS)  main.c -L. -Wl,-rpath=. -llib -o main
	make run_all

clean:
	@rm -rf *.o *.so *.a main

clean_raport:
	@rm -f $(FILENAME)

clean_w:
	@rm -rf w*.txt

run_all:
	make clean_w
	@echo "\n n:100 blockLength:0.01 \n" >> $(FILENAME)
	./main 0.01 100 >> $(FILENAME)

	make clean_w
	@echo "\n n:1000 blockLength:0.001 \n" >> $(FILENAME)
	./main 0.001 1000 >> $(FILENAME)

	make clean_w
	@echo "\n n:10000 blockLength:0.0001 \n" >> $(FILENAME)
	./main 0.0001 10000 >> $(FILENAME)

	make clean_w
	@echo "\n n:100 blockLength:0.008 \n" >> $(FILENAME)
	./main 0.008 100 >> $(FILENAME)

	make clean_w
	@echo "\n n:1000 blockLength:0.0008 \n" >> $(FILENAME)
	./main 0.0008 1000 >> $(FILENAME)

	make clean_w
	@echo "\n n:100 blockLength:0.005 \n" >> $(FILENAME)
	./main 0.005 100 >> $(FILENAME)

	make clean_w
	@echo "\n n:1000 blockLength:0.0005 \n" >> $(FILENAME)
	./main 0.0005 1000 >> $(FILENAME)

	make clean_w
	@echo "\n n:5 blockLength:0.1 \n" >> $(FILENAME)
	./main 0.1 5 >> $(FILENAME)




