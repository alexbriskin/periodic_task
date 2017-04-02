.PHONY: all clean

all:
	gcc *.c -lrt -pthread -o periodic.o

clean:
	rm *.o
