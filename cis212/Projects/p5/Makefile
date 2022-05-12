CFLAGS=-g -W -Wall -I/usr/local/include/
LDFLAGS=-L/usr/local/lib
LIBRARIES=-lADTs
PROGRAM=insideout
OBJECTS=insideout.o addresslist.o
MANIFEST="addresslist.c\ninsideout.c\nalinsideout.c\nMakefile\nreport.txt"

all: $(PROGRAM)

insideout: $(OBJECTS)
	gcc $(CFLAGS) $(LDFLAGS) -o $(PROGRAM) $(OBJECTS) $(LIBRARIES)

addresslist.o: addresslist.h addresslist.c

insideout.o: insideout.c addresslist.h

clean:
	rm -f *.o $(PROGRAM)

zip:
	$(shell echo -e $(MANIFEST) >manifest)
	tar -zcvf rheise-project5.tgz $(shell cat manifest)