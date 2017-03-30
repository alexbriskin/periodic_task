.PHONY: all clean

all:
	gcc *.c -lrt -o periodic.o

clean:
	rm *.o
