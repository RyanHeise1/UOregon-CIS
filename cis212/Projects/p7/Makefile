CFLAGS=-g -W -Wall -g -I/usr/local/include
LDFLAGS=-L/usr/local/lib
LIBRARIES=-lADTs
PROGRAM=motorcycleTour motorcycleTourSingle
MANIFEST="motorcycleTour.c\nllistqueue.c\nMakefile\nreport.txt\n"

all: $(PROGRAM)

motorcycleTour: motorcycleTour.o
	gcc $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

motorcycleTourSingle: motorcycleTour.o llistqueue.o
	gcc $(CFLAGS) -o $@ $^ $(LIBRARIES)

motorcycleTour.o: motorcycleTour.c

llistqueue.o: llistqueue.c

clean:
	rm -f *.o $(PROGRAM)

zip:
	$(shell echo -e $(MANIFEST) >manifest)
	tar -zcvf rheise-project7.tgz $(shell cat manifest)