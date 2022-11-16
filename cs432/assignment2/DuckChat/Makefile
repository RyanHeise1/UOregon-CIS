CC=g++
CFLAGS=-Wall -W -g -Werror 
# Uncomment this line for the graduate student version
#CFLAGS= -g  -DGRAD=1
#LOADLIBES= -lnsl

all: client server

client: client.cc raw.cc
	$(CC) client.cc raw.cc $(LOADLIBES) $(CFLAGS) -o client

server: server.cc 
	$(CC) server.cc $(LOADLIBES) $(CFLAGS) -o server

clean:
	rm -f client server *.o

