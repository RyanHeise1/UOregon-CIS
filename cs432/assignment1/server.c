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
#include <errno.h>
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

typedef struct node {
    char *username; // name of user
    char *channels[BUFSIZ]; // holds list of joined channels for users
    struct sockaddr_in addr;
    int index; // value of next index in channels
    int client_port;
    struct node *next; // next value in linked list
}Node;

typedef struct list {
    Node *head;
}List;

Node *create_node(char *username, struct sockaddr_in addr, int client_port) {
    Node *p = (Node *)malloc(sizeof(Node));
    if (!p){
        printf("Error creating node\n");
        exit(EXIT_FAILURE);
    }
    p->username = username;
    p->username = malloc(sizeof(char) * USERNAME_MAX);
    
    p->addr = addr;
    p->index = 0;
    p->client_port = client_port;
    p->next = NULL;
    return p;
}

void add_node(List *l, char *username, struct sockaddr_in addr, int client_port){
    Node *p = NULL;
    if (l->head == NULL){
        l->head = create_node(username, addr, client_port);
    }else{
        p = l->head;
        while (p->next != NULL){
            p = p->next;
        }
        p->next = create_node(username, addr, client_port);
    }
}

Node *find_node(List *l, int client_port){
    Node *p = NULL;
    if (l->head == NULL){
        printf("Error finding node. List empty\n");
        return NULL;
    }else{
        p = l->head;
        while (p != NULL){
            if (p->client_port == client_port){
                return p;
            }
            p = p->next;
        }
        printf("Error finding user in list\n");
        return NULL;
    }
}

void delete_node(List *l, int client_port){
    Node *current = l->head;
    Node *prev = current;
    while (current != NULL){
        if (current->client_port == client_port){
            prev->next = current->next;
            if (current == l->head){
                l->head = current->next;
            }
            free(current->username);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

List *create_list(){
    List *l = (List *)malloc(sizeof(List));
    if (!l){
        fprintf(stderr, "Error creating list\n");
        exit(EXIT_FAILURE);
    }
    l->head = NULL;
    return l;
}

int remove_str(Node *user, char *key){
    // remove key from arr
    int index = -1;
    int arrLen = 0;

    /*dont really need this while loop since we keep track of 
      user->index (which is the length)*/
    while (user->channels[arrLen] != NULL){
        arrLen++;
    }

    // search for index of key
    for (int i = 0; i < arrLen; i++){
        if ((strcmp(user->channels[i], key)) == 0){
            index = i;
            break;
        }
    }
    if (index == -1){
        printf("Error finding index of %s in list\n", key);
        return -1;
    }
    // update array
    for(int j = index-1; j < arrLen-1; j++){
        user->channels[j] = user->channels[j+1];
    }
    return 1;
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

bool find_duplicate(char *arr[], char *key){
    int i = 0;
    bool found = false;
    while (arr[i] != NULL){
        if ((strcmp(arr[i], key)) == 0){
            found = true;
        }
    }
    return found;
}


int main(int argc, char* argv[]){
    char *nptr; // Used to Convert value to a decimal long
    int port; // argv[2]
    char ip[BUFSIZ]; // IP address of hostname (obtained in hostname_to_ip())
    int client_port;
    int socketFd;
    struct sockaddr_in serverAddr, clientAddr;

    List *l = NULL;
    l = create_list(); // create list to store nodes

    txt_say.txt_type = TXT_SAY;
    txt_who.txt_type = TXT_WHO;
    txt_error.txt_type = TXT_ERROR;
        
    //fprintf(stdout, "Check arguments\n");
    if (argc != 3){
        fprintf(stderr, "Usage: ./server host_address port_number\n");
        exit(EXIT_FAILURE);
    }
    char *username = argv[3];
    /* Convert the provided value to a decimal long */
    //fprintf(stdout, "Check port len\n");
    port = strtol(argv[2], &nptr, 10);
    // valid port numbers in range 1-65535
    // https://en.wikipedia.org/wiki/List_of_TCP_and_UDP_port_numbers
    if (port <= 0 || port > 65535){
        fprintf(stderr, "Invalid port. Enter number > 0\n");
        exit(EXIT_FAILURE);
    }
    hostname_to_ip(argv[1], ip);
    //printf("%s resolved to %s\n" , argv[1] , ip);

    //fprintf(stdout, "Create socket\n");
    socketFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socketFd == -1){
        fprintf(stderr, "ERROR: server socket creation failed... exiting\n");
        exit(EXIT_FAILURE);
    }
    memset(&serverAddr, 0, sizeof(serverAddr));
    memset(&clientAddr, 0, sizeof(clientAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port); //translate between host byte order and network byte
    serverAddr.sin_addr.s_addr = inet_addr(ip);

    //printf("Bind\n");
    // resource: https://www.ibm.com/docs/en/zos/2.3.0?topic=functions-bind-bind-name-socket
    int bindClient = bind(socketFd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    if (bindClient == -1){
        fprintf(stderr, "ERROR: unable to bind... exiting\n");
        printf("Error code: %d\n", errno);
        exit(EXIT_FAILURE);
    }
    fprintf(stdout, "Server initiated\nClients can now connect\n");
    
    socklen_t fromClientLen = sizeof(clientAddr);
    struct sockaddr_in tmpAddr;
    char *tmp; // temp value that is used in recieve (function takes a char *)
    while (true){
        recvfrom(socketFd, (void *) &tmp, sizeof(tmp), 0, (struct sockaddr *) &clientAddr, &fromClientLen);
        req = (struct request *) &tmp; // cast value of tmp to the request str
        switch(req->req_type){
            case REQ_LOGIN:
                req_login = (struct request_login *) &tmp;
                client_port = ntohs(clientAddr.sin_port);
                // new client joined. Add them to the list
                add_node(l, username, clientAddr, client_port); 
                Node *login_usr = find_node(l, client_port);
                login_usr->channels[login_usr->index++] = "Common";
                fprintf(stdout, "User %s joined on port %d\n", req_login->req_username, client_port);
                break;
            case REQ_LOGOUT:
                req_logout = (struct request_logout *) &tmp;
                client_port = ntohs(clientAddr.sin_port);
                Node *usr_logout = find_node(l, client_port);
                delete_node(l, client_port);
                fprintf(stdout, "User %s logged out\n", usr_logout->username);
                break;
            case REQ_JOIN:
                bool found = false;
                req_join = (struct request_join *) &tmp;
                client_port = ntohs(clientAddr.sin_port);
                Node *usr_join = find_node(l, client_port);
                for (int i = 0; i < usr_join->index; i++){
                    if (usr_join->channels[i] == req_join->req_channel){
                        found = true;
                    }
                    i++;
                }
                if (found){
                    fprintf(stdout, "User is already in %s\n", req_join->req_channel);
                }else{
                    usr_join->channels[usr_join->index] = req_join->req_channel;
                    usr_join->index++;
                }
                break;
            case REQ_LEAVE:
                req_leave = (struct request_leave *) &tmp;
                client_port = ntohs(clientAddr.sin_port);
                //printf("Leave: %d\n", client_port);
                Node *usr_leave = find_node(l, client_port);
                remove_str(usr_leave, req_leave->req_channel);
                fprintf(stdout, "User %s has left %s\n", usr_leave->username, req_leave->req_channel);
                break;
            case REQ_SAY:
                req_say = (struct request_say *) &tmp;
                client_port = ntohs(clientAddr.sin_port);
                Node *usr_say = find_node(l, client_port);
                printf("Say\n");
                strcpy(txt_say.txt_channel, req_say->req_channel);
                strcpy(txt_say.txt_username, usr_say->username);
                strcpy(txt_say.txt_text, req_say->req_text);
                
                Node *p = l->head;
                while (p != NULL){
                    if (find_duplicate(p->channels, req_say->req_channel)){
                        sendto(socketFd, (void *) &txt_say, sizeof(txt_say), 0, (struct sockaddr *) &tmpAddr, sizeof(tmpAddr));
                    }
                    p = p->next;
                }
                // for items in list
                    // if user is in channel
                        // sendto
                
                break;
            case REQ_LIST:
                printf("req_list\n");
                //req_list = (struct request_list *) &tmp;
                //client_port = ntohs(clientAddr.sin_port);
                
                break;
            case REQ_WHO:
                char *who_arr[BUFSIZ];
                int who_i = 0;
                req_who = (struct request_who *) &tmp;
                //client_port = ntohl(clientAddr.sin_port);
                Node *p = l->head;
                char *key = req_who->req_channel;
                while (p != NULL){ // loop through all elem in list
                    for (int i = 0; i < p->index; i++){ // loop through channels in node
                        // if key is channel[i], add it to the list
                        if ((strcmp(key, p->channels[i])) == 0){
                            who_arr[who_i++] = p->username;
                        }
                    }
                    p = p->next;
                }
                // after this, we should have a list to send to client requesting
                //sendto();
                printf("who\n");
                break;
            default:
                fprintf(stderr, "Error parsing packet type (unknown)\n");
                break;
        }
    }
    return 0;
}