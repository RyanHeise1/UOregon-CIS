CFLAGS=-g -w -Wall
PROGRAMS= lab3
MANIFEST= "lab3.c\nMakefile\nlab3_sc.png"

all = $(PROGRAMS)

lab3: lab3.o
	gcc -o lab3 $^

lab3.0: lab3.c

clean:
	rm -f $(PROGRAMS) *.o

zip:
	$(shell echo -e $(MANIFEST) >manifest)
	tar -zcvf lab3.tgz $(shell cat manifest)