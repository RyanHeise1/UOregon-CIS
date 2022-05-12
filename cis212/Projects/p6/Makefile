CFLAGS=-W -Wall -g -I/usr/local/include
LIBRARIES=-lADTs
LDFLAGS=-L/usr/local/lib
PROGRAMS=exerciseStack postfixIt
OBJECTS=exerciseStack.o postfixIt.o
MANIFEST="exerciseStack.c\npostfixIt.c\nMakefile\nreport.txt"

all: $(PROGRAMS)

exerciseStack: exerciseStack.o
	gcc $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

postfixIt: postfixIt.o
	gcc $(CFLAGS) -o $@ $^ $(LIBRARIES)

exerciseStack.o: exerciseStack.c

postfixIt.o:postfixIt.c

clean:
	rm -f *.o $(PROGRAMS)

zip:
	$(shell echo -e $(MANIFEST) >manifest)
	tar -zcvf rheise-project6.tgz $(shell cat manifest)