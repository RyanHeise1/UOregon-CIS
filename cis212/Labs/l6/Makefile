CFLAGS=-g -W -Wall -I/sur/local/include/
LDFLAGS=-L/usr/local/lib
LIBRARIES=-lADTs
PROGRAM=lab6
OBJECTS=main.o student.o studentlist.o

all: $(PROGRAM)

lab6: $(OBJECTS)
	gcc $(CFLAGS) $(LDFLAGS) -o $(PROGRAM) $(OBJECTS) $(LIBRARIES)

main.o: main.c student.h studentlist.h

student.o: student.c student.h

studentlist.o: studentlist.c studentlist.h student.h

clean:
	rm -f *.o $(PROGRAM)