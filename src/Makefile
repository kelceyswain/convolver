CC=gcc
CFLAGS=-Wall -O3 -c
CLIBS=`pkg-config --libs sndfile`

all: convolver

convolver: main.o normalize.o splitchans.o
	$(CC) main.o normalize.o splitchans.o -o convolver $(CLIBS)

main.o: main.c
	$(CC) $(CFLAGS) main.c

normalize.o: normalize.c
	$(CC) $(CFLAGS) normalize.c

splitchans.o: splitchans.c
	$(CC) $(CFLAGS) splitchans.c

clean:
	rm -rf *.o convolver
