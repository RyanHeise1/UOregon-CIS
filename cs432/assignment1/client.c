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
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include "duckchat.h"
#include "raw.h"

// all structs from duckchat.h
struct request_login request_login;
struct request_logout request_logout;
struct request_join request_join;
struct request_leave request_leave;
struct request_say request_say;
struct request_list request_list;
struct request_who request_who;
struct text *message;
struct text_say *text_say; 
struct text_list *text_list;
struct text_who *text_who;
struct text_error *text_error;

// globals
char *active_channel; // holds the active channel you are talking o
char fromServer[BUFSIZ]; // message from server 
int socketFd; // fd for socket

int process_message(){
    char message[SAY_MAX];
    char letter;
    int i = 0;

    // Takes in user input letter by letter
    // stops at SAY_MAX
    raw_mode();
    printf("\n>");
    while((letter = getchar()) != '\n'){
        printf("%c", letter);
        message[i++] = letter;
    }
    cooked_mode();

    if (message[0] == '/'){
        char split[2] = " ";
        char *token;
        char *arg; // arguments for join, leave, who, switch

        token = strtok(message, split); // firs token is the /command
        while (token != NULL){
            if ((strcmp(token, "/exit")) == 0){
                if ((strtok(NULL, split)) != NULL){
                    fprintf(stderr, "invalid command\n");
                    exit(EXIT_FAILURE);
                }else{
                    send(socketFd, (void *) &request_logout, sizeof(request_logout), 0);
                    close(socketFd);
                    return EXIT_SUCCESS;
                }
            }
            if ((strcmp(token, "/list")) == 0){
                if ((strtok(NULL, split)) != NULL){
                    fprintf(stderr, "invalid command\n");
                    exit(EXIT_FAILURE);
                }else{
                    send(socketFd, (void *) &request_list, sizeof(request_list), 0);
                }
            }
            if ((strcmp(token, "/join")) == 0){
                if ((token = strtok(NULL, split)) == NULL){ // if there is no argument, throw error
                    fprintf(stderr, "invalid command\n");
                    exit(EXIT_FAILURE);
                }
                else if ((arg = strtok(NULL, split)) != NULL){ // 2 arguments were given, throw error and exit
                    fprintf(stderr, "invalid command\n");
                    exit(EXIT_FAILURE);
                }else{ 
                    strcpy(request_join.req_channel, token);
                    send(socketFd, (void *) &request_join, sizeof(request_join), 0);
                    // might have to keep track of subscribed channels here
                    active_channel = token;
                }
            }
            if ((strcmp(token, "/leave")) == 0){
                if ((token = strtok(NULL, split)) == NULL){
                    fprintf(stderr, "invalid command\n");
                    exit(EXIT_FAILURE);
                }else if ((arg = strtok(NULL, split)) != NULL){ // 2 arguments were given, throw error and exit
                    fprintf(stderr, "invalid command\n");
                    exit(EXIT_FAILURE);
                }else{
                    strcpy(request_leave.req_channel, token);
                    send(socketFd, (void *) &request_leave, sizeof(request_leave), 0);
                    printf("TODO");
                }
            }
            if ((strcmp(token, "/who")) == 0){
                if ((token = strtok(NULL, split)) == NULL){
                    fprintf(stderr, "invalid command\n");
                    exit(EXIT_FAILURE);
                }else if ((arg = strtok(NULL, split)) != NULL){ // 2 arguments were given, throw error and exit
                    fprintf(stderr, "invalid command\n");
                    exit(EXIT_FAILURE);
                }else{
                    strcpy(request_who.req_channel, token);
                    send(socketFd, (void *) &request_who, sizeof(request_who), 0);
                    printf("TODO");
                }
            }
            if ((strcmp(token, "/switch")) == 0){
                if ((token = strtok(NULL, split)) == NULL){
                    fprintf(stderr, "invalid command\n");
                    exit(EXIT_FAILURE);
                }else if ((arg = strtok(NULL, split)) != NULL){ // 2 arguments were given, throw error and exit
                    fprintf(stderr, "invalid command\n");
                    exit(EXIT_FAILURE);
                }else{
                    printf("TODO");
                }
            }
        }
    }
    else{
        // Set up the request_say struct and send message to server
        strcpy(request_say.req_channel, active_channel);
        strcpy(request_say.req_text, message);
        int to = send(socketFd, (void *) &request_say, sizeof(request_say), 0);
        if (to == -1){ // check is send failed
            fprintf(stderr, "Failed to send message to server");
            exit(EXIT_FAILURE);
        }
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

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

int main(int argc, char *argv[]) {
    char *hostname; // argv[1]
    int port; // argv[2]
    char *username; // argv[3]
    char ip[BUFSIZ]; // IP address of hostname (obtained in hostname_to_ip())
    struct sockaddr_in toServer;

    // When user first joins, their active channel is "Common"
    active_channel = "Common";

    // info to send to server
    request_login.req_type = REQ_LOGIN;
    strcpy(request_login.req_username, username);
    request_logout.req_type = REQ_LOGOUT;
    request_join.req_type = REQ_JOIN;
    strcpy(request_join.req_channel, active_channel);
    request_leave.req_type = REQ_LEAVE;
    request_say.req_type = REQ_SAY;
    request_list.req_type = REQ_LIST;
    request_who.req_type = REQ_WHO;

    // check number of arguments... Exit if < 3
    if (argc < 3){                    
        fprintf(stderr, "Usage: ./client server_socket server_port username\n");
        exit(EXIT_FAILURE);
    }
    // (unneccessary) set variables so I dont have to use argv[i] everytime
    hostname = argv[1];
    port = atoi(argv[2]);
    username = strdup(argv[3]);
    // check that username is > the max length defined in duckchat.h (size = 32)
    if (strlen(username) > USERNAME_MAX){
        fprintf(stderr, "ERROR: Username Lenght should be >= 32");
        exit(EXIT_FAILURE);
    }

    // convert hostname to IPv4 address 
    // (if user enters in "localhost" as the hostname, it will resolve to IPv4)
    hostname_to_ip(hostname, ip);
    // printf("%s resolved to %s" , hostname , ip);

    // create socket
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd == -1){
        fprintf(stderr, "ERROR: Socket creation failed... exiting");
        exit(EXIT_FAILURE);
    }
    // https://www.gta.ufrj.br/ensino/eel878/sockets/sockaddr_inman.html
    memset(&toServer, 0, sizeof(toServer));
    toServer.sin_family = AF_INET;
    toServer.sin_port = htonl(port); //translate between host byte order and network byte

    // connect to server
    int connectServer = connect(socketFd, (struct sockaddr *)&toServer, sizeof(toServer));
    if (connectServer == -1){
        fprintf(stderr, "Failed to connect to server\n");
        exit(EXIT_FAILURE);
    }

    // request login
    int login = send(socketFd, (void *) &request_login, sizeof(request_login), 0);
    if (login == -1){
        fprintf(stderr, "Failed to send login request\n");
        exit(EXIT_FAILURE);
    }
    
    // If login is successful, we join with the username
    int join = send(socketFd, (void *) &request_join, sizeof(request_join), 0);
    if (join == -1){
        fprintf(stderr, "Failed to send join request\n");
        exit(EXIT_FAILURE);
    }
    // if login successful, we can now send messages to the server
    while (true){
        // process stdin input and send to server using socketFd
        process_message();

        // wait to recieve message back from server
        int from = recv(socketFd, &fromServer, sizeof(fromServer), 0);
        if (from == -1){ // if not able to recieve message
            fprintf(stderr, "Not able to recieve message from server\n");
            exit(EXIT_FAILURE);
        }else{
            message = (struct text *) &fromServer;
            switch(message->txt_type){
                case TXT_SAY: // print output
                    text_say = (struct text_say *) &fromServer;
                    printf("[%s][%s]: %s\n", text_say->txt_channel, text_say->txt_username, text_say->txt_text);
                    break;
                case TXT_LIST: // list all channels
                    text_list = (struct text_list *) &fromServer;
                    printf("Existing channels:\n");
                    for (int i = 0; i < text_list->txt_nchannels; i++){
                        printf("\t%s", text_list->txt_channels[i].ch_channel);
                    }
                    printf("\n");
                    break;
                case TXT_WHO:
                    text_who = (struct text_who *) &fromServer;
                    printf("Users in channel %s:", text_who->txt_channel);
                    for (int i = 0; i < text_who->txt_nusernames; i++){
                        printf("\t%s", text_who->txt_users[i].us_username);
                    }
                    printf("\n");
                    break;
                case TXT_ERROR:
                    // TODO: What to do here?
                    break;
                default:
                    fprintf(stderr, "Error parsing packet type (unknown)\n");
                    break;
            }
        }
    }
    close(socketFd);
}