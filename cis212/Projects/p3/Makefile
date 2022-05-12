CFLAGS=-W -Wall
PROGRAMS=sgrep
MANIFEST = "sgrep.c\nMakefile\nreport.txt"

all: $(PROGRAMS)

sgrep: sgrep.o
	gcc -o $@ $^

sgrep.o: sgrep.c

clean:
	rm -f $(PROGRAMS) *.o

zip:
	$(shell echo -e $(MANIFEST) >manifest)
	tar -zcvf rheise-project3.tgz $(shell cat manifest)