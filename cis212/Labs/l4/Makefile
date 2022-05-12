CFLAGS=-g 
PROGRAMS=lab4
MANIFEST="lab4.c\nMakefile\nscreenshot.png"

all: $(PROGRAMS)

lab4: lab4.o
	gcc -o lab4 $^

lab4.o: lab4.c

clean:
	rm -f $(PROGRAMS) *.o

tgz:
	$(shell echo -e ${MANIFEST} >manifest)
	tar -zcvf lab4.tgz $(shell cat manifest)
