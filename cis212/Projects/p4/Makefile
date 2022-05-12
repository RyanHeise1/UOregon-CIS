CFLAGS=-W -Wall -I/usr/local/include
LDFLAGS=-L/usr/local/lib
LIBRARIES=-lADTs
MANIFEST = "addressmgr.c\nMakefile\nreport.txt"

addressmgr: addressmgr.o
	gcc $(LDFLAGS) -o $@ $^ $(LIBRARIES)

addressmgr.o: addressmgr.c

clean:
	rm -f addressmgr addressmgr.o

zip:
	$(shell echo -e $(MANIFEST) >manifest)
	tar -zcvf rheise-project4.tgz $(shell cat manifest)
