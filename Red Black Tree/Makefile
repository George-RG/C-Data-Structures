CC=gcc
objects= demo.o RB.o
headers= RB.h
CFLAGS=-g -Wall -Werror -std=c99

main:$(objects)
	$(CC) $(objects) -o demo

RB.o: $(headers) 

demo.o: $(headers)

PHONY clean:
	rm -f $(objects) demo