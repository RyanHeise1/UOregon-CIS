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
struct request_login req_login;
struct request_logout req_logout;
struct request_join req_join;
struct request_leave req_leave;
struct request_say req_say;
struct request_list req_list;
struct request_who req_who;
struct text *msg;
struct text_say *txt_say; 
struct text_list *txt_list;
struct text_who *txt_who;
struct text_error *txt_error;

// globals
char *active_channel; // holds the active channel you are talking o
char fromServer[BUFSIZ]; // message from server 
char *listening_channels[CHANNEL_MAX] = {};
int num_channel = 0;
int socketFd; // fd for socket
struct sockaddr_in serverAddr;

int find_index(char *arg){
    // find index of arg in listening_channels
    int index = -1;
    int arrLen = 0;
    while (listening_channels[arrLen] != NULL){
        arrLen++;
    }
    // search for index of key
    for (int i = 0; i < arrLen; i++){
        if ((strcmp(listening_channels[i], arg)) == 0){
            index = i;
            break;
        }
    }
    return index;
}

void remove_str(char **arr, char *key){
    // remove key from arr
    int index = -1;
    int arrLen = 0;
    while (listening_channels[arrLen] != NULL){
        arrLen++;
    }
    // search for index of key
    for (int i = 0; i < arrLen; i++){
        if ((strcmp(arr[i], key)) == 0){
            index = i;
            break;
        }
    }
    // update array
    for(int j = index-1; j < arrLen-1; j++){
        arr[j] = arr[j+1];
    }
}

void process_message(){
    fprintf(stdout, "Process message\n");
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
    message[i] = '\0';

    if (message[0] == '/'){
        char split[2] = " ";
        char *token;
        char arg[BUFSIZ]; // arguments for join, leave, who, switch

        token = strtok(message, split); // firs token is the /command
        if ((strcmp(token, "/exit")) == 0){
            printf("Exit detected\n");
            if ((strtok(NULL, split)) != NULL){
                fprintf(stderr, "invalid command\n");
            }else{
                sendto(socketFd, (void *) &req_logout, sizeof(req_logout), MSG_CONFIRM, (const struct sockaddr *) &serverAddr, sizeof(serverAddr));
                close(socketFd);
            }
        }
        else if ((strcmp(token, "/list")) == 0){
            printf("List detected\n");
            if ((strtok(NULL, split)) != NULL){
                fprintf(stderr, "invalid command\n");
                exit(EXIT_FAILURE);
            }else{
                sendto(socketFd, (void *) &req_list, sizeof(req_list), MSG_CONFIRM, (const struct sockaddr *) &serverAddr, sizeof(serverAddr));
            }
        }
        else if ((strcmp(token, "/join")) == 0){
            printf("Join detected\n");
            if ((token = strtok(NULL, split)) == NULL){ // if there is no argument, throw error
                fprintf(stderr, "invalid command\n");
            }else{
                while(token != NULL){
                    strcat(arg, token);
                    strcat(arg, " ");
                    token = strtok(NULL, split);
                }
                strcpy(req_join.req_channel, arg);
                sendto(socketFd, (void *) &req_join, sizeof(req_join), MSG_CONFIRM, (const struct sockaddr *) &serverAddr, sizeof(serverAddr));
                listening_channels[num_channel++] = arg;
                active_channel = arg;
            }
        }
        else if ((strcmp(token, "/leave")) == 0){
            printf("Leave detected\n");
            if ((token = strtok(NULL, split)) == NULL){
                fprintf(stderr, "invalid command\n");
            }else{
                while(token != NULL){
                    strcat(arg, token);
                    strcat(arg, " ");
                    token = strtok(NULL, split);
                }
                strcpy(req_leave.req_channel, arg);
                sendto(socketFd, (void *) &req_leave, sizeof(req_leave), MSG_CONFIRM, (const struct sockaddr *) &serverAddr, sizeof(serverAddr));
                remove_str(listening_channels, arg);
                num_channel--;
            }
        }
        else if ((strcmp(token, "/who")) == 0){
            if ((token = strtok(NULL, split)) == NULL){
                fprintf(stderr, "invalid command\n");
            }else{
                while(token != NULL){
                    strcat(arg, token);
                    strcat(arg, " ");
                    token = strtok(NULL, split);
                }
                strcpy(req_who.req_channel, arg);
                sendto(socketFd, (void *) &req_who, sizeof(req_who), MSG_CONFIRM, (const struct sockaddr *) &serverAddr, sizeof(serverAddr));
            }
        }
        else if ((strcmp(token, "/switch")) == 0){
            printf("Switch detected\n");
            if ((token = strtok(NULL, split)) == NULL){
                fprintf(stderr, "invalid command\n");
            }else{
                while(token != NULL){
                    strcat(arg, token);
                    strcat(arg, " ");
                    token = strtok(NULL, split);
                }
                int index = find_index(arg);
                if (index != -1){
                    active_channel = arg;
                }else{
                    fprintf(stderr, "You are not subscribed to %s\n", arg);
                }
            }
        }
        else{
            fprintf(stderr, "Unknown command\n");
        }
    }
    else{
        // Set up the request_say struct and send message to server
        strcpy(req_say.req_channel, active_channel);
        strcpy(req_say.req_text, message);
        int to = sendto(socketFd, (void *) &req_say, sizeof(req_say), MSG_CONFIRM, (const struct sockaddr *) &serverAddr, sizeof(serverAddr));
        if (to == -1){ // check is send failed
            fprintf(stderr, "Failed to send message to server");
        }
    }
}

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

int main(int argc, char *argv[]) {
    fprintf(stdout, "start\n");
    char *hostname; // argv[1]
    char *nptr;
    char *tmp;
    char *username; // argv[3]
    char ip[BUFSIZ]; // IP address of hostname (obtained in hostname_to_ip())
    int port; // argv[2]
    socklen_t len;

    // When user first joins, their active channel is "Common"
    active_channel = "Common";
    listening_channels[num_channel++] = active_channel;

    // info to send to server
    req_login.req_type = REQ_LOGIN;
    req_logout.req_type = REQ_LOGOUT;
    req_join.req_type = REQ_JOIN;
    strcpy(req_join.req_channel, active_channel);
    req_leave.req_type = REQ_LEAVE;
    req_say.req_type = REQ_SAY;
    req_list.req_type = REQ_LIST;
    req_who.req_type = REQ_WHO;

    // check number of arguments... Exit if < 3
    fprintf(stdout, "Check Args\n");
    if (argc < 3){                    
        fprintf(stderr, "Usage: ./client server_socket server_port username\n");
        exit(EXIT_FAILURE);
    }
    // (unneccessary) set variables so I dont have to use argv[i] everytime
    hostname = argv[1];
    port = strtol(argv[2], &nptr, 10);
    username = strdup(argv[3]);
    strcpy(req_login.req_username, username);
    fprintf(stdout, "Check username len\n");
    // check that username is > the max length defined in duckchat.h (size = 32)
    if (strlen(username) > USERNAME_MAX){
        fprintf(stderr, "ERROR: Username Lenght should be >= 32");
        exit(EXIT_FAILURE);
    }

    // convert hostname to IPv4 address 
    // (if user enters in "localhost" as the hostname, it will resolve to IPv4)
    hostname_to_ip(hostname, ip);
    printf("%s resolved to %s\n" , hostname , ip);

    // create socket
    fprintf(stdout, "Create socket\n");
    socketFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socketFd == -1){
        fprintf(stderr, "ERROR: client socket creation failed... exiting");
        exit(EXIT_FAILURE);
    }
    // https://www.gta.ufrj.br/ensino/eel878/sockets/sockaddr_inman.html
    fprintf(stdout, "memset\n");
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port); //translate between host byte order and network byte
    serverAddr.sin_addr.s_addr = inet_addr(ip);

    // request login
    fprintf(stdout, "request login\n");
    // TODO USE SEND TO
    int login = sendto(socketFd, (void *) &req_login, sizeof(req_login), MSG_CONFIRM, (const struct sockaddr *) &serverAddr, sizeof(serverAddr));
    //int login = sendto(socketFd, &req_login, sizeof(req_login), 0);
    if (login == -1){
        fprintf(stderr, "Failed to send login request\n");
        exit(EXIT_FAILURE);
    }
    
    // If login is successful, we join with the username
    fprintf(stdout, "Join\n");
    int join = sendto(socketFd, (void *) &req_join, sizeof(req_join), MSG_CONFIRM, (const struct sockaddr *) &serverAddr, sizeof(serverAddr));
    if (join == -1){
        fprintf(stderr, "Failed to send join request\n");
        exit(EXIT_FAILURE);
    }
    // if login successful, we can now send messages to the server
    while (true){
        // process stdin input and send to server using socketFd
        process_message();

        // wait to recieve message back from server
        int from = recvfrom(socketFd, (void *) &tmp, sizeof(tmp), MSG_WAITALL, (struct sockaddr *) &serverAddr, &len);
        //int from = recvfrom(socketFd, &fromServer, sizeof(fromServer), 0);
        if (from == -1){ // if not able to recieve message
            fprintf(stderr, "Not able to recieve message from server\n");
        }else{
            msg = (struct text *) &fromServer;
            switch(msg->txt_type){
                case TXT_SAY: // print output
                    txt_say = (struct text_say *) &fromServer;
                    printf("[%s][%s]: %s\n", txt_say->txt_channel, txt_say->txt_username, txt_say->txt_text);
                    break;
                case TXT_LIST: // list all channels
                    txt_list = (struct text_list *) &fromServer;
                    printf("Existing channels:\n");
                    for (int i = 0; i < txt_list->txt_nchannels; i++){
                        printf("\t%s", txt_list->txt_channels[i].ch_channel);
                    }
                    printf("\n");
                    break;
                case TXT_WHO:
                    txt_who = (struct text_who *) &fromServer;
                    printf("Users in channel %s:", txt_who->txt_channel);
                    for (int i = 0; i < txt_who->txt_nusernames; i++){
                        printf("\t%s", txt_who->txt_users[i].us_username);
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