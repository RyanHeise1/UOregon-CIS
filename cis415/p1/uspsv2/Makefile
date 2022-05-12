CFLAGS = -W -Wall
OBJECTS = uspsv2.o p1fxns.o
sources = uspsv2.c p1fxns.c
CC = gcc


target = uspsv2

all: $(target)

$(target) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $(target) $(sources)

uspsv2.o : uspsv2.c p1fxns.h

p1fxns.o : p1fxns.c p1fxns.h

clean:
	rm -rf $(target) $(OBJECTS)
