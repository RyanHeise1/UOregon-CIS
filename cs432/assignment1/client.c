/*
.Must take exactly three command-line arguments
    1) hostname where the server is running
    2) port number on which the server is listening
    3) user's username.

.When client starts, it automatically connects to the chat server, joins a channel called “Common”, 
    and provides the user a prompt

.When Client enteres a message, server relays message to all clients including the speaker

.If text begins with "/" it is interpereted as a special command
    1) /exit: Logout the user and exit the client software.
    2) /join channel: Join (subscribe in) the named channel, creating the channel if it does not exist.
    3) /leave channel: Leave the named channel.
    4) /list: List the names of all channels.
    5) /who channel: List the users who are on the named channel.
    6) /switch channel: Switch to an existing named channel that the user has already joined
        .If the switch command fails, the active channel should remain unchanged

.User can listen to multiple channels at the same time but can only be active on one channel at a time
    .most recently joined channel by a user is always the active channel

.client should keep track of the active channel for the user
    ./switch command does not require sending any message to the server
    ./join command requires the client to send a message to the server.

.client should detect when the user tries to switch to a channel to which it has not already subscribed 
    and give an error message to the user

.chat format:
    [channel][username]: text

.client application should print many backspace characters ('\b' in C) to erase the prompt and anything 
    the local user has typed


RESOURCES:
    . https://www.ibm.com/docs/en/zos/2.3.0?topic=functions-socket-create-socket
*/
#include <stdio.h> // BUFSIZ
#include <stdlib.h>  //EXIT_SUCCESS/FAILURE
#include <stdbool.h> // bools
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include "duckchat.h"

// all structs from duckchat.h
struct request_login request_login;
struct request_logout request_logout;
struct request_join request_join;
struct request_leave request_leave;
struct request_say request_say;
struct request_list request_list;
struct request_who request_who;
struct text text;
struct text_say text_say; 
struct text_list text_list;
struct text_who text_who;
struct text_error text_error;

// globals
char *list[] = {}; // holds the channels you are listening on
char *active_channel; // holds the active channel you are talking on

// resource: https://www.binarytides.com/hostname-to-ip-address-c-sockets-linux/
//           https://www.ibm.com/docs/en/ztpf/1.1.0.14?topic=zf-gethostbyname-get-ip-address-information-by-host-name
int hostname_to_ip(char *hostname, char *ip){
    struct hostent *hp; // struct returned by gethostbyname()
    struct in_addr **addr_list; 

    if ((hp = gethostbyname(hostname)) == NULL){ // if gethostbyname() returns a NULL pointer throw errpr
        fprintf(stderr, "Error resolving hostname: %s\n", hostname);
        return EXIT_FAILURE;
    } else{
        addr_list = (struct in_addr **)hp->h_addr_list; // vector of addresses for the host
        strcpy(ip, inet_ntoa(*addr_list[0])); // take first element in addr_list and cpy it to "ip"
        return EXIT_SUCCESS;
    }
}

int main(int argc, char *argv[]) {
    char *hostname; // argv[1]
    int port; // argv[2]
    char *username; // argv[3]
    int socketFd; // fd for socket
    char ip[BUFSIZ]; // IP address of hostname (obtained in hostname_to_ip())

    // check number of arguments... Exit if < 3
    if (argc < 3){                    
        fprintf(stderr, "Usage: ./client server_socket server_port username\n");
        exit(-1);
    }
    // (unneccessary) set variables so I dont have to use argv[i] everytime
    hostname = argv[1];
    port = atoi(argv[2]);
    username = argv[3];
    // check that username is > the max length defined in duckchat.h (size = 32)
    if (strlen(username) > USERNAME_MAX){
        fprintf(stderr, "ERROR: Username Lenght should be >= 32");
        return EXIT_FAILURE;
    }

    // convert hostname to IPv4 address 
    // (if user enters in "localhost" as the hostname, it will resolve to IPv4)
    hostname_to_ip(hostname, ip);
    // printf("%s resolved to %s" , hostname , ip);

    // create socket
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd == -1){
        fprintf(stderr, "ERROR: Socket creation failed... exiting");
        return EXIT_FAILURE;
    }

    // use memset to create socket
}