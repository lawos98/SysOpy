CC = gcc
CFLAGS = -Wall -Werror -g
FILENAME=raport.txt

runAll:
	make runFORK
	make runEXEC

runEXEC:
	make clean
	$(CC) $(CFLAGS)  main.c -o main -ldl -D EXEC
	@echo "EXEC ignore" >>$(FILENAME)
	./main ignore >> $(FILENAME)
	@echo "EXEC mask" >>$(FILENAME)
	./main mask >> $(FILENAME)
	@echo "EXEC pending" >>$(FILENAME)
	./main pending >> $(FILENAME)

runFORK:
	make clean
	$(CC) $(CFLAGS)  main.c -o main -ldl -D FORK
	@echo "FORK ignore" >>$(FILENAME)
	./main ignore >> $(FILENAME)
	@echo "FORK mask" >>$(FILENAME)
	./main mask >> $(FILENAME)
	@echo "FORK pending" >>$(FILENAME)
	./main pending >> $(FILENAME)
	@echo "FORK handle" >>$(FILENAME)
	./main handle >> $(FILENAME)

clean:
	@rm -rf *.o *.so *.a main

clean_raport:
	@rm -f $(FILENAME)


