CC = gcc
CFLAGS = -Wall -g

FILE= raport.txt
INPUT= gator.ascii.pgm
OUTPUT= output.ascii.pgm

all: run clean

compile:
	$(CC) $(FLAGS) main.c -o main -pthread

run:
	make compile
	./main 1 numbers $(INPUT) $(OUTPUT) >> $(FILE)
	./main 2 numbers $(INPUT) $(OUTPUT) >> $(FILE)
	./main 4 numbers $(INPUT) $(OUTPUT) >> $(FILE)
	./main 8 numbers $(INPUT) $(OUTPUT) >> $(FILE)
	./main 16 numbers $(INPUT) $(OUTPUT) >> $(FILE)

	@echo "\n\n" >> $(FILE)

	./main 1 block $(INPUT) $(OUTPUT) >> $(FILE)
	./main 2 block $(INPUT) $(OUTPUT) >> $(FILE)
	./main 4 block $(INPUT) $(OUTPUT) >> $(FILE)
	./main 8 block $(INPUT) $(OUTPUT) >> $(FILE)
	./main 16 block $(INPUT) $(OUTPUT) >> $(FILE)

run_test:
	make compile
	./main 4 block $(INPUT) $(OUTPUT) >> $(FILE)

clean:
	rm -f raport.txt
	rm -f $(OUTPUT)
	rm -f main