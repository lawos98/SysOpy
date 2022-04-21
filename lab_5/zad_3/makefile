main:
	gcc -c -Wall producer.c -o producer
	gcc -c -Wall consument.c -o consument

testSend:
	make main
	mkfifo fifo
	./producer fifo 1 test1.txt 10
	./producer fifo 1 test2.txt 10
	./producer fifo 1 test3.txt 10
	./producer fifo 1 test4.txt 10
	./consument fifo output.txt 10

clean:
	rm *.o main