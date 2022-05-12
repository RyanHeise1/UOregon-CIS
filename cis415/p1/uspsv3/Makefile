CFLAGS = -W -Wall -I/usr/local/include -g
LDFLAGS=-L/usr/local/lib
LIBRARIES=-lADTs
PROGS=uspsv3

all: $(PROGS)

uspsv3 : uspsv3.o p1fxns.o $(LIBRARIES)
	gcc $(LDFLAGS) -o $@ $^

uspsv3.o : uspsv3.c p1fxns.h

p1fxns.o : p1fxns.c p1fxns.h

clean:
	rm -rf $(PROGS) *.o
