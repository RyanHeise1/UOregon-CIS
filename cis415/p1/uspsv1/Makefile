CFLAGS = -W -Wall
OBJECTS = uspsv1.o p1fxns.o
sources = uspsv1.c p1fxns.c
CC = gcc


target = uspsv1

all: $(target)

$(target) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $(target) $(sources)

uspsv1.o : uspsv1.c p1fxns.h

p1fxns.o : p1fxns.c p1fxns.h

clean:
	rm -rf $(target) $(OBJECTS)
