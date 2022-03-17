CC = gcc
CFLAGS = -Wall -Werror -g
O ?= O0
FILENAME=raport.txt

run_static:
	make clean
	$(CC) $(CFLAGS) -$(O)  -c lib.c -o lib.o
	ar rcs liblib.a lib.o
	$(CC) $(CFLAGS) -$(O) main.c -llib -L./ -o main
	make -s run_test


run_shared:
	make clean
	$(CC) $(CFLAGS) -$(O) -fPIC -shared lib.c -o liblib.so
	$(CC) $(CFLAGS) -$(O) main.c -L. -Wl,-rpath=. -llib -o main
	make -s run_test


run_dynamic:
	make clean
	$(CC) $(CFLAGS) -$(O) -fPIC -shared lib.c -o liblib.so
	$(CC) $(CFLAGS) -$(O) main.c -ldl -D DYNAMIC -o main
	make -s run_test

clean:
	@rm -rf *.o *.so *.a main

clean_raport:
	@rm -f raport*.txt

run_test:
	@echo "test 1: \n" >> $(FILENAME)
	./main `cat script/script1.txt` >> $(FILENAME)

	@echo "test 2: \n" >> $(FILENAME)
	./main `cat script/script2.txt` >> $(FILENAME)

	@echo "test 3: \n" >> $(FILENAME)
	./main `cat script/script3.txt` >> $(FILENAME)

	@echo "test 4: \n" >> $(FILENAME)
	./main `cat script/script4.txt` >> $(FILENAME)

run_all:
	@echo "static O0: \n" >> $(FILENAME)
	make run_static O=O0

	@echo "static O1: \n" >> $(FILENAME)
	make run_static O=O1

	@echo "static O2: \n" >> $(FILENAME)
	make run_static O=O2

	@echo "static O3: \n" >> $(FILENAME)
	make run_static O=O3

	@echo "shared O0: \n" >> $(FILENAME)
	make run_shared O=O0

	@echo "shared O1: \n" >> $(FILENAME)
	make run_shared O=O1

	@echo "shared O2: \n" >> $(FILENAME)
	make run_shared O=O2

	@echo "shared O3: \n" >> $(FILENAME)
	make run_shared O=O3

	@echo "dynamic O0: \n" >> $(FILENAME)
	make run_dynamic O=O0

	@echo "dynamic O1: \n" >> $(FILENAME)
	make run_dynamic O=O1

	@echo "dynamic O2: \n" >> $(FILENAME)
	make run_dynamic O=O2

	@echo "dynamic O3: \n" >> $(FILENAME)
	make run_dynamic O=O3