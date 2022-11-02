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

struct request *req;
struct request_login *req_login;
struct request_logout *req_logout;
struct request_join *req_join;
struct request_leave *req_leave;
struct request_say *req_say;
struct request_list *req_list;
struct request_who *req_who;
struct text_say txt_say;
struct channel_info *chn_info;
struct text_list *txt_list;
struct text_who txt_who;
struct text_error txt_error; 

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
    fprintf(stdout, "Hostname to ip\n");
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
    fprintf(stdout, "Test\n");
    char *nptr; // Used to Convert value to a decimal long
    int port; // argv[2]
    char ip[BUFSIZ]; // IP address of hostname (obtained in hostname_to_ip())
    //int client_port;
    int socketFd;
    struct sockaddr_in serverAddr, clientAddr;

    txt_say.txt_type = TXT_SAY;
    txt_who.txt_type = TXT_WHO;
    txt_error.txt_type = TXT_ERROR;
        
    fprintf(stdout, "Check arguments\n");
    if (argc != 3){
        fprintf(stderr, "Usage: ./server host_address port_number\n");
        exit(EXIT_FAILURE);
    }
    /* Convert the provided value to a decimal long */
    fprintf(stdout, "Check port len\n");
    port = strtol(argv[2], &nptr, 10);
    // valid port numbers in range 1-65535
    // https://en.wikipedia.org/wiki/List_of_TCP_and_UDP_port_numbers
    if (port <= 0 || port > 65535){
        fprintf(stderr, "Invalid port. Enter number > 0\n");
        exit(EXIT_FAILURE);
    }
    hostname_to_ip(argv[1], ip);
    printf("%s resolved to %s\n" , argv[1] , ip);

    fprintf(stdout, "Create socket\n");
    socketFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFd == -1){
        fprintf(stderr, "ERROR: server socket creation failed... exiting\n");
        exit(EXIT_FAILURE);
    }
    memset(&serverAddr, 0, sizeof(serverAddr));
    memset(&clientAddr, 0, sizeof(clientAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port); //translate between host byte order and network byte
    serverAddr.sin_addr.s_addr = inet_addr(ip);

    printf("Bind\n");
    // resource: https://www.ibm.com/docs/en/zos/2.3.0?topic=functions-bind-bind-name-socket
    int bindClient = bind(socketFd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    if (bindClient == -1){
        fprintf(stderr, "ERROR: unable to bind... exiting\n");
        exit(EXIT_FAILURE);
    }
    
    socklen_t fromClientLen = sizeof(clientAddr);
    char *tmp; // temp value that is used in recieve (function takes a char *)
    while (true){
        recvfrom(socketFd, (void *) &tmp, sizeof(tmp), MSG_WAITALL, (struct sockaddr *) &clientAddr, &fromClientLen);
        req = (struct request *) &tmp; // cast value of tmp to the request str
        if (clientAddr.sin_family == AF_INET){
            switch(req->req_type){
                case REQ_LOGIN:
                    req_login = (struct request_login *) &tmp;
                    //client_port = ntohl(fromClient.sin_port);
                    printf("login");
                    break;
                case REQ_LOGOUT:
                    req_logout = (struct request_logout *) &tmp;
                    //client_port = ntohl(fromClient.sin_port);
                    printf("logout");
                    break;
                case REQ_JOIN:
                    req_join = (struct request_join *) &tmp;
                    //client_port = ntohl(fromClient.sin_port);
                    printf("join");
                    break;
                case REQ_LEAVE:
                    req_leave = (struct request_leave *) &tmp;
                    //client_port = ntohl(fromClient.sin_port);
                    printf("leave");
                    break;
                case REQ_SAY:
                    req_say = (struct request_say *) &tmp;
                    //client_port = ntohl(fromClient.sin_port);
                    printf("say");
                    break;
                case REQ_LIST:
                    req_list = (struct request_list *) &tmp;
                    //client_port = ntohl(fromClient.sin_port);
                    printf("list");
                    break;
                case REQ_WHO:
                    req_who = (struct request_who *) &tmp;
                    //client_port = ntohl(fromClient.sin_port);
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