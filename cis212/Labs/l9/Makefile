CFLAGS=-g -W -Wall -I/usr/local/include
LDFLAGS=-L/usr/local/lib
LIBR=-lADTs
MANIFEST="lab9.c\nMakefile\nlotr1\nsort.c\nsort.h\nlab9.png"

lab9: lab9.o sort.o
	gcc $(LDFLAGS) -o $@ $^ $(LIBR)

lab9.o: lab9.c

sort.o: sort.c sort.h

clean:
	rm -f *.o lab9

run:
	./lab9 lotr1

zip:
	$(shell echo -e $(MANIFEST) >manifest)
	tar -zcvf lab9.tgz $(shell cat manifest)