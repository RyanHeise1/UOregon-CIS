CFLAGS=-g -W -Wall -I/usr/local/include
LDFLAGS=-L/usr/local/lib
LIBRARIES=-lADTs
MANIFEST="lab8.c\nMakefile\nlab8.png"

lab8: lab8.o
	gcc $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

lab8.o:lab8.c

clean:
	rm -f *.o lab8

zip:
	$(shell echo -e $(MANIFEST) >manifest)
	tar -zcvf lab8.tgz $(shell cat manifest)