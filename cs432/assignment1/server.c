/*
.DuckChat uses UDP to communicate
    .run the client and server on the same machine and not worry about dropping packets or 
    out-of-order packets.

.server takes two arguments:
    1) host address to bind
        .can be either:
            .'localhost'
                .if you use 'localhost', you can't connect to the server from another machine
            .IP hostname of the machine that the server is running on
            .IP address of an interface connected to the machine
    2) port number

.recommended that the server outputs debugging messages to the console
    .each time the server receives a message from a client, it can output a line describing the 
    contents of the message and who it is from using the following format: [channel][user][message]

.server must keep track of individual users and the channels they have subscribed to
    .server must also track each channel and the subscribed users on it.

.Channel creation and deletion at the server are handled implicitly
    .no users, it is deleted
*/
#include <stdio.h> // BUFSIZ
#include <stdlib.h>  //EXIT_SUCCESS/FAILURE
#include <unistd.h>
#include <stdbool.h> // bools
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include "duckchat.h"
#include "raw.h"

struct request *request;
struct request_login *request_login;
struct request_logout *request_logout;
struct request_join *request_join;
struct request_leave *request_leave;
struct request_say *request_say;
struct request_list *request_list;
struct request_who *request_who;
struct text_say text_say;
struct channel_info *channel_info;
struct text_list *text_list;
struct text_who *text_who;
struct text_error *text_error; 

/*struct user {
    char *username;
    char *ip;
} User;

struct channel {
    char *channels[CHANNEL_MAX];
    struct User;
}ActiveChannels;*/

// resource: https://www.binarytides.com/hostname-to-ip-address-c-sockets-linux/
//           https://www.ibm.com/docs/en/ztpf/1.1.0.14?topic=zf-gethostbyname-get-ip-address-information-by-host-name
int hostname_to_ip(char *hostname, char *ip){
    struct hostent *hp; // struct returned by gethostbyname()
    struct in_addr **addr_list; 

    if ((hp = gethostbyname(hostname)) == NULL){ // if gethostbyname() returns a NULL pointer throw errpr
        fprintf(stderr, "Error resolving hostname: %s\n", hostname);
        exit(EXIT_FAILURE);
    } else{
        addr_list = (struct in_addr **)hp->h_addr_list; // vector of addresses for the host
        strcpy(ip, inet_ntoa(*addr_list[0])); // take first element in addr_list and cpy it to "ip"
        return EXIT_SUCCESS;
    }
}

int main(int argc, char* argv[]){
    char *nptr; // Used to Convert value to a decimal long
    char *tmp; // temp value that is used in recieve (function takes a char *)
    int port; // argv[2]
    //int client_port;
    int server_socket;
    struct sockaddr_in server;
    struct sockaddr_in fromSrc;
    
    socklen_t fromSrcLen = sizeof(fromSrc);
    
    text_say.txt_type = TXT_SAY;
    text_who->txt_type = TXT_WHO;
    text_error->txt_type = TXT_ERROR;
    
    if (argc != 3){
        fprintf(stderr, "Usage: ./server host_address port_number\n");
        exit(EXIT_FAILURE);
    }
    
    /* Convert the provided value to a decimal long */
    port = strtol(argv[2], &nptr, 10);
    // valid port numbers in range 1-65535
    // https://en.wikipedia.org/wiki/List_of_TCP_and_UDP_port_numbers
    if (port <= 0 || port > 65535){
        fprintf(stderr, "Invalid port. Enter number > 0");
        exit(EXIT_FAILURE);
    }
    printf("Create socket");
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1){
        fprintf(stderr, "ERROR: server socket creation failed... exiting");
        exit(EXIT_FAILURE);
    }
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port); //translate between host byte order and network byte
    
    printf("Bind");
    // resource: https://www.ibm.com/docs/en/zos/2.3.0?topic=functions-bind-bind-name-socket
    int bindClient = bind(server_socket, (struct sockaddr *) &server, sizeof(server));
    if (bindClient == -1){
        fprintf(stderr, "ERROR: unable to bind... exiting");
        exit(EXIT_FAILURE);
    }
    while (true){
        recvfrom(server_socket, (void *) &tmp, sizeof(tmp), 0, (struct sockaddr *) &fromSrc, &fromSrcLen);
        request = (struct request *) &tmp; // cast value of tmp to the request str
        if (fromSrc.sin_family == AF_INET){
            switch(request->req_type){
                case REQ_LOGIN:
                    request_login = (struct request_login *) &tmp;
                    //client_port = ntohl(fromSrc.sin_port);
                    printf("login");
                    break;
                case REQ_LOGOUT:
                    request_logout = (struct request_logout *) &tmp;
                    //client_port = ntohl(fromSrc.sin_port);
                    printf("logout");
                    break;
                case REQ_JOIN:
                    request_join = (struct request_join *) &tmp;
                    //client_port = ntohl(fromSrc.sin_port);
                    printf("join");
                    break;
                case REQ_LEAVE:
                    request_leave = (struct request_leave *) &tmp;
                    //client_port = ntohl(fromSrc.sin_port);
                    printf("leave");
                    break;
                case REQ_SAY:
                    request_say = (struct request_say *) &tmp;
                    //client_port = ntohl(fromSrc.sin_port);
                    printf("say");
                    break;
                case REQ_LIST:
                    request_list = (struct request_list *) &tmp;
                    //client_port = ntohl(fromSrc.sin_port);
                    printf("list");
                    break;
                case REQ_WHO:
                    request_who = (struct request_who *) &tmp;
                    //client_port = ntohl(fromSrc.sin_port);
                    printf("who");
                    break;
                default:
                    fprintf(stderr, "Error parsing packet type (unknown)\n");
                    break;
            }
        }
    }
    return 0;
}