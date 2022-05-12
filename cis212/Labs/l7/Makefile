CFLAGS=-W -Wall
MANIFEST="exerciseLList.c\nMakefile\ninput\nLab7ss.png"

exerciseLList: exerciseLList.o
	gcc $(CFLAGS) -o $@ $^

exerciseLList.o: exerciseLList.c

clean:
	rm -f exerciseLList exerciseLList.o

zip:
	$(shell echo -e $(MANIFEST) >manifest)
	tar -zcvf lab7.tgz $(shell cat manifest)