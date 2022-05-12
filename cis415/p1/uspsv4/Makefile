CFLAGS = -W -Wall -I/usr/local/include -g
LDFLAGS=-L/usr/local/lib
LIBRARIES=-lADTs
PROGS=uspsv4

all: $(PROGS)

uspsv4 : uspsv4.o p1fxns.o $(LIBRARIES)
	gcc $(LDFLAGS) -o $@ $^

uspsv4.o : uspsv4.c p1fxns.h

p1fxns.o : p1fxns.c p1fxns.h

clean:
	rm -rf $(PROGS) *.o
