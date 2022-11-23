#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <time.h>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;
//#include "hash.h"
#include "duckchat.h"


#define MAX_CONNECTIONS 10
#define HOSTNAME_MAX 100
#define MAX_MESSAGE_LEN 65536

//typedef map<string,string> channel_type; //<username, ip+port in string>
// Each server already keeps track of which users are in each channel. 
//Now, the server must additionally note if any adjacent servers are subscribed to a channel
struct users{
	map<string,struct sockaddr_in> channel_type; //<username, sockaddr_in of user>
	map<string, struct sockaddr_in> channel_servers; // server name and sockaddr of server
};

int s; //socket for listening
struct sockaddr_in server;


map<string,struct sockaddr_in> usernames; //<username, sockaddr_in of user>
map<string,int> active_usernames; //0-inactive , 1-active
//map<struct sockaddr_in,string> rev_usernames;
map<string,string> rev_usernames; //<ip+port in string, username>
map<string,users> channels; // <name of channel, usernames/sockaddr>
map<string,struct sockaddr_in>server_mp; //<ip, sockaddr_in of server>
map<string,int>active_servers; //0-inactive , 1-active

void handle_socket_input();
void handle_login_message(void *data, struct sockaddr_in sock);
void handle_logout_message(struct sockaddr_in sock);
void handle_join_message(void *data, struct sockaddr_in sock);
void handle_leave_message(void *data, struct sockaddr_in sock);
void handle_say_message(void *data, struct sockaddr_in sock);
void handle_list_message(struct sockaddr_in sock);
void handle_who_message(void *data, struct sockaddr_in sock);
void handle_keep_alive_message(struct sockaddr_in sock);
void send_error_message(struct sockaddr_in sock, string error_msg);

/* Functions to handle Server -> Server message */
void process_ss_join_message(void *data, struct sockaddr_in sock);
void process_ss_leave_message(void *data, struct sockaddr_in sock);
/*void process_ss_say_message(void *data, struct sockaddr_in sock);*/
void send_join(string id, string channel_name, struct sockaddr_in sock);
void send_say(string channel, string username, string text, unsigned long long int id, struct sockaddr_in send_sock);

// Globals
int timer = 60; /* Timer for server to renue subscription */
string server_ip_port;
std::vector<unsigned long long int> users_id;

int main(int argc, char *argv[]){
	
	/* We know argc has to be > 3 from P1. In P2, the ammount of args should be an odd number of args */
	//printf("DEBUG: check argc\n");
	if (((argc + 1) % 2) != 0 || argc < 3){ 
		printf("Usage: ./server ix 5000 ix 5001\n");
		exit(1);
	}

	char hostname[HOSTNAME_MAX];
	string ip;
	int port;
	
	strcpy(hostname, argv[1]);
	port = atoi(argv[2]);
	
	//printf("DEBUG: create socket\n");
	s = socket(PF_INET, SOCK_DGRAM, 0);
	if (s < 0){
		perror ("socket() failed\n");
		exit(1);
	}

	struct sockaddr_in server;
	struct hostent *he;

	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	//printf("DEBUG: get hostname\n");
	if ((he = gethostbyname(hostname)) == NULL) {
		puts("error resolving hostname... exiting\n");
		exit(1);
	}

	memcpy(&server.sin_addr, he->h_addr_list[0], he->h_length);
	ip = inet_ntoa(server.sin_addr);
	server_ip_port = ip + "." + to_string(server.sin_port); /* Format IP.Port */
	//printf("DEBUG: ip.port = %s\n", server_ip_port.c_str());
	//printf("DEBUG: creating map for %s\n", server_ip_port.c_str());
	struct sockaddr_in tmp;
	for (int i = 3; i < argc; i += 2){ /* We start at 3 since we are doing bind on the first 2 elem below */
		
		strcpy(hostname, argv[i]);
		port = atoi(argv[i+1]);

		if ((he = gethostbyname(hostname)) == NULL) {
			puts("error resolving hostname... exiting\n");
			exit(1);
		}	
		tmp.sin_family = AF_INET;
		tmp.sin_port = htons(port);

		memcpy(&tmp.sin_addr, he->h_addr_list[0], he->h_length);
		ip = inet_ntoa(tmp.sin_addr);
		server_ip_port = ip + "." + to_string(tmp.sin_port);
		//printf("MAP DEBUG: ip.port = %s\n", server_ip_port.c_str());
		//printf("MAP DEBUG: adding server to map and setting active\n");
		server_mp[server_ip_port] = tmp; /* Add server to tree */
		active_servers[server_ip_port] = 0; /* Set server to active */
	}
	printf("//\n\nMAP DEBUG: map created for %s\n\n", server_ip_port.c_str());

	int err;

	//printf("DEBUG: binding socket\n");
	err = bind(s, (struct sockaddr*)&server, sizeof(server));
	if (err < 0){
		perror("bind failed\n");
	}
	else{
		//printf("bound socket\n");
	}

	//testing maps end
	//create default channel Common
	//string default_channel = "Common";
	//struct users;
	//map<string,struct sockaddr_in> default_channel_users;
	//users.channel_servers[default_channel] = default_channel_users

	while(1) {
		//use a file descriptor with a timer to handle timeouts
		int rc;
		fd_set fds;

		FD_ZERO(&fds);
		FD_SET(s, &fds);
		
		rc = select(s+1, &fds, NULL, NULL, NULL);
		if (rc < 0){
			printf("error in select\n");
            getchar();
		}
		else{
			//int socket_data = 0;

			if (FD_ISSET(s,&fds)){
				//reading from socket
				handle_socket_input();
				//socket_data = 1;
			}
		}	
	} 
	return 0;
}

void handle_socket_input(){ 
	struct sockaddr_in recv_client;
	ssize_t bytes;
	void *data;
	size_t len;
	socklen_t fromlen;
	fromlen = sizeof(recv_client);
	char recv_text[MAX_MESSAGE_LEN];
	data = &recv_text;
	len = sizeof recv_text;


	bytes = recvfrom(s, data, len, 0, (struct sockaddr*)&recv_client, &fromlen);


	if (bytes < 0){
		perror ("recvfrom failed\n");
	}
	else{
		//printf("received message\n");

		struct request* request_msg;
		request_msg = (struct request*)data;

		//printf("Message type:");
		request_t message_type = request_msg->req_type;

		//printf("%d\n", message_type);

		if (message_type == REQ_LOGIN){
			printf("DEBUG: REQ_LOGIN\n");
			handle_login_message(data, recv_client); //some methods would need recv_client
		}
		else if (message_type == REQ_LOGOUT){
			printf("DEBUG: REQ_LOGOUT\n");
			handle_logout_message(recv_client);
		}
		else if (message_type == REQ_JOIN){
			printf("DEBUG: REQ_JOIN\n");
			handle_join_message(data, recv_client);
		}
		else if (message_type == REQ_LEAVE){
			printf("DEBUG: REQ_LEAVE\n");
			handle_leave_message(data, recv_client);
		}
		else if (message_type == REQ_SAY){
			printf("DEBUG: REQ_SAY\n");
			handle_say_message(data, recv_client);
		}
		else if (message_type == REQ_LIST){
			printf("DEBUG: REQ_LIST\n");
			handle_list_message(recv_client);
		}
		else if (message_type == REQ_WHO){
			printf("DEBUG: REQ_WHO\n");
			handle_who_message(data, recv_client);
		}
		else if (message_type == REQ_SS_JOIN){
			printf("DEBUG: REQ_SS_JOIN\n");
			process_ss_join_message(data, recv_client);
		}
		else if (message_type == REQ_SS_LEAVE){
			printf("DEBUG: REQ_SS_LEAVE\n");
			//process_ss_leave_message(data, recv_client);
		}
		else if (message_type == REQ_SS_SAY){
			printf("DEBUG: REQ_SS_SAY\n");
			//process_ss_say_message(data, recv_client);
		}
		else{
			//send error message to client
			send_error_message(recv_client, "*Unknown command");
		}
	}
}

void process_ss_join_message(void *data, struct sockaddr_in sock){
	//get message fields
	struct request_join* msg;
	msg = (struct request_join*)data;

	string channel = msg->req_channel;

	string ip = inet_ntoa(sock.sin_addr);
	int port = sock.sin_port;

 	char port_str[6];
 	sprintf(port_str, "%d", port);
	string key = ip + "." + port_str;

	//check whether key is in map of users and adj servers
	map <string,struct users> :: iterator iter;
	map<string, struct sockaddr_in> new_channel;
	iter = channels.find(key);
	if (iter == channels.end() ){
		//ip+port not recognized - create new channel and broadcast to other servers
		new_channel[key] = sock;
		channels[channel].channel_servers = new_channel;
		// loop through servers and send join message
		map<string,struct sockaddr_in>::iterator sock_iter;
		for (sock_iter = server_mp.begin(); sock_iter != server_mp.end(); sock_iter++){
			string server_ip_port = sock_iter->first;
			struct sockaddr_in sock = sock_iter->second;
			send_join(server_ip_port, channel, sock);
		}
	}
	else{
		// found key in channels map
		channels[channel].channel_servers[key] = sock;
	}
	cout << server_ip_port << " " << key << " recieved S2S Join from " << channel << endl;
}

void process_ss_leave_message(void *data, struct sockaddr_in sock){
	//get message fields
	struct request_ss_leave* msg;
	msg = (struct request_ss_leave*)data;

	string channel = msg->req_channel;

	string ip = inet_ntoa(sock.sin_addr);
	int port = sock.sin_port;

 	char port_str[6];
 	sprintf(port_str, "%d", port);
	string key = ip + "." +port_str;

	//check whether key is in rev_usernames
	map <string,struct users> :: iterator iter;
	//map<string,struct users>::iterator channel_iter;
	iter = channels.find(key);
	if (iter == channels.end()) {
		// ip.port not recognized
		send_error_message(sock, "Not logged in"); 
	}else{
		// channel was found
		// find server id
		// same concept as in handle_logout_message()
		map<string,struct sockaddr_in>::iterator within_channel_iterator;
		// get user from map
		struct users usr = iter->second;
		// search for user
		within_channel_iterator = usr.channel_type.find(key);
		if (within_channel_iterator != iter->second.channel_type.end()){
			iter->second.channel_type.erase(within_channel_iterator);
		}else{
			cout << "unable to find sockaddr of " << channel << endl;
		}

	}
	cout << server_ip_port << " " << key << " recieved S2S Leave from " << channel << endl;
}

/*void process_ss_say_message(void *data, struct sockaddr_in sock){
	
}*/

void handle_login_message(void *data, struct sockaddr_in sock){
	struct request_login* msg;
	msg = (struct request_login*)data;

	string username = msg->req_username;
	usernames[username]	= sock;
	active_usernames[username] = 1;

	//rev_usernames[sock] = username;

	//char *inet_ntoa(struct in_addr in);
	string ip = inet_ntoa(sock.sin_addr);
	//cout << "ip: " << ip <<endl;
	int port = sock.sin_port;
	//unsigned short short_port = sock.sin_port;
	//cout << "short port: " << short_port << endl;
	//cout << "port: " << port << endl;

 	char port_str[6];
 	sprintf(port_str, "%d", port);
	//cout << "port: " << port_str << endl;

	string key = ip + "." +port_str;
	//cout << "key: " << key <<endl;
	rev_usernames[key] = username;

	cout << "server: " << username << " logs in" << endl;
}

void handle_logout_message(struct sockaddr_in sock){

	//construct the key using sockaddr_in
	string ip = inet_ntoa(sock.sin_addr);
	//cout << "ip: " << ip <<endl;
	int port = sock.sin_port;

 	char port_str[6];
 	sprintf(port_str, "%d", port);
	//cout << "port: " << port_str << endl;

	string key = ip + "." +port_str;
	//cout << "key: " << key <<endl;

	//check whether key is in rev_usernames
	map <string,string> :: iterator iter;

	/*
    for(iter = rev_usernames.begin(); iter != rev_usernames.end(); iter++)
    {
        cout << "key: " << iter->first << " username: " << iter->second << endl;
    }
	*/

	iter = rev_usernames.find(key);
	if (iter == rev_usernames.end() ){
		//send an error message saying not logged in
		send_error_message(sock, "Not logged in");
	}
	else{
		//cout << "key " << key << " found."<<endl;
		string username = rev_usernames[key];
		rev_usernames.erase(iter);

		//remove from usernames
		map<string,struct sockaddr_in>::iterator user_iter;
		user_iter = usernames.find(username);
		usernames.erase(user_iter);

		//remove from all the channels if found
		map<string,struct users>::iterator channel_iter;
		for(channel_iter = channels.begin(); channel_iter != channels.end(); channel_iter++){
			//cout << "key: " << iter->first << " username: " << iter->second << endl;
			//channel_type current_channel = channel_iter->second;
			map<string,struct sockaddr_in>::iterator within_channel_iterator;
			within_channel_iterator = channel_iter->second.channel_type.find(username);
			if (within_channel_iterator != channel_iter->second.channel_type.end()){
				channel_iter->second.channel_type.erase(within_channel_iterator);
			}
		}

		//remove entry from active usernames also
		//active_usernames[username] = 1;
		map<string,int>::iterator active_user_iter;
		active_user_iter = active_usernames.find(username);
		active_usernames.erase(active_user_iter);

		cout << "server: " << username << " logs out" << endl;
	}


	/*
    for(iter = rev_usernames.begin(); iter != rev_usernames.end(); iter++)
    {
        cout << "key: " << iter->first << " username: " << iter->second << endl;
    }
	*/


	//if so delete it and delete username from usernames
	//if not send an error message - later

}

void handle_join_message(void *data, struct sockaddr_in sock){
	//get message fields
	struct request_join* msg;
	msg = (struct request_join*)data;

	string channel = msg->req_channel;

	string ip = inet_ntoa(sock.sin_addr);

	int port = sock.sin_port;

 	char port_str[6];
 	sprintf(port_str, "%d", port);
	string key = ip + "." +port_str;


	//check whether key is in rev_usernames
	map <string,string> :: iterator iter;


	iter = rev_usernames.find(key);
	if (iter == rev_usernames.end() ){
		//ip+port not recognized - send an error message
		send_error_message(sock, "Not logged in");
	}
	else{
		string username = rev_usernames[key];

		map<string,struct users>::iterator channel_iter;

		channel_iter = channels.find(channel);

		active_usernames[username] = 1;
		/* When a user joins a channel
			user server checks to see if already subscribed to channel 
			if user is already in channel
				no additional steps
			else
				server finds other servers subscribed to that channel */
		if (channel_iter == channels.end()){
			//channel not found
			map<string,struct sockaddr_in> new_channel_users;
			new_channel_users[username] = sock;
			channels[channel].channel_type = new_channel_users;
			//cout << "creating new channel and joining" << endl;
			map<string,struct sockaddr_in>::iterator sock_iter;

			// loop through servers and send join message
			for (sock_iter = server_mp.begin(); sock_iter != server_mp.end(); sock_iter++){
				string server_ip_port = sock_iter->first;
				send_join(server_ip_port, channel, sock_iter->second);
			}
		}
		else{
			//channel already exits
			//map<string,struct sockaddr_in>* existing_channel_users;
			//existing_channel_users = &channels[channel];
			//*existing_channel_users[username] = sock;

			channels[channel].channel_type[username] = sock;
			//cout << "joining exisitng channel" << endl;
		}
		cout << "server: " << username << " joins channel " << channel << endl;
	}
	//check whether the user is in usernames
	//if yes check whether channel is in channels
	//if channel is there add user to the channel
	//if channel is not there add channel and add user to the channel
}

void send_join(string id, string channel_name, struct sockaddr_in sock){
	// sends join msg to all adj servers
	void *send_data;
	size_t len;
	ssize_t bytes;
	struct request_ss_join req_join;

	// set up request struct
	req_join.req_type = REQ_SS_JOIN;
	strcpy(req_join.req_channel, channel_name.c_str());

	send_data = &req_join;
	len = sizeof(req_join);
	bytes = sendto(s, send_data, len, 0, (struct sockaddr*)&sock, sizeof sock);

	//check to see that it was sent correctly
	if (bytes < 0){ // send failed
		cout << "Message to adj server failed" << endl;
	}else{
		cout << server_ip_port << " " << id << " sent S2S join msg to " << channel_name << endl;
	}
}

void handle_leave_message(void *data, struct sockaddr_in sock){

	//check whether the user is in usernames
	//if yes check whether channel is in channels
	//check whether the user is in the channel
	//if yes, remove user from channel
	//if not send an error message to the user


	//get message fields
	struct request_leave* msg;
	msg = (struct request_leave*)data;

	string channel = msg->req_channel;

	string ip = inet_ntoa(sock.sin_addr);

	int port = sock.sin_port;

 	char port_str[6];
 	sprintf(port_str, "%d", port);
	string key = ip + "." +port_str;


	//check whether key is in rev_usernames
	map <string,string> :: iterator iter;


	iter = rev_usernames.find(key);
	if (iter == rev_usernames.end() ){
		//ip+port not recognized - send an error message
		send_error_message(sock, "Not logged in");
	}
	else{
		string username = rev_usernames[key];

		map<string,struct users>::iterator channel_iter;

		channel_iter = channels.find(channel);

		active_usernames[username] = 1;

		if (channel_iter == channels.end()){
			//channel not found
			send_error_message(sock, "No channel by the name " + channel);
			cout << "server: " << username << " trying to leave non-existent channel " << channel << endl;

		}
		else{
			//channel already exits
			//map<string,struct sockaddr_in> existing_channel_users;
			//existing_channel_users = channels[channel];
			map<string,struct sockaddr_in>::iterator channel_user_iter;
			channel_user_iter = channels[channel].channel_type.find(username);

			if (channel_user_iter == channels[channel].channel_type.end()){
				//user not in channel
				send_error_message(sock, "You are not in channel " + channel);
				cout << "server: " << username << " trying to leave channel " << channel  << " where he/she is not a member" << endl;
			}
			else{
				channels[channel].channel_type.erase(channel_user_iter);
				//existing_channel_users.erase(channel_user_iter);
				cout << "server: " << username << " leaves channel " << channel <<endl;

				//delete channel if no more users
				if (channels[channel].channel_type.empty() && (channel != "Common")){
					channels.erase(channel_iter);
					cout << "server: " << "removing empty channel " << channel <<endl;
				}
			}
		}
	}
}




void handle_say_message(void *data, struct sockaddr_in sock){

	//check whether the user is in usernames
	//if yes check whether channel is in channels
	//check whether the user is in the channel
	//if yes send the message to all the members of the channel
	//if not send an error message to the user


	//get message fields
	struct request_say* msg;
	msg = (struct request_say*)data;

	string channel = msg->req_channel;
	string text = msg->req_text;


	string ip = inet_ntoa(sock.sin_addr);

	int port = sock.sin_port;

 	char port_str[6];
 	sprintf(port_str, "%d", port);
	string key = ip + "." + port_str;


	//check whether key is in rev_usernames
	map <string,string> :: iterator iter;
	iter = rev_usernames.find(key);

	if (iter == rev_usernames.end() ){
		//ip+port not recognized - send an error message
		send_error_message(sock, "Not logged in ");
	}
	else{
		string username = rev_usernames[key];

		map<string,struct users>::iterator channel_iter;

		channel_iter = channels.find(channel);

		active_usernames[username] = 1;

		if (channel_iter == channels.end()){
			//channel not found
			send_error_message(sock, "No channel by the name " + channel);
			cout << "server: " << username << " trying to send a message to non-existent channel " << channel << endl;
		}
		else{
			//channel already exits
			//map<string,struct sockaddr_in> existing_channel_users;
			//existing_channel_users = channels[channel];
			map<string,struct sockaddr_in>::iterator channel_user_iter;
			channel_user_iter = channels[channel].channel_type.find(username);

			if (channel_user_iter == channels[channel].channel_type.end()){
				//user not in channel
				send_error_message(sock, "You are not in channel " + channel);
				cout << "server: " << username << " trying to send a message to channel " << channel  << " where he/she is not a member" << endl;
			}
			else {
				// user in channel
				// loop through users in channel and send msg
				map<string,struct sockaddr_in> existing_channel_users = channels[channel].channel_type;
				for(channel_user_iter = existing_channel_users.begin(); channel_user_iter != existing_channel_users.end(); channel_user_iter++){
					//cout << "key: " << iter->first << " username: " << iter->second << endl;

					ssize_t bytes;
					void *send_data;
					size_t len;

					struct text_say send_msg;
					send_msg.txt_type = TXT_SAY;

					const char* str = channel.c_str();
					strcpy(send_msg.txt_channel, str);
					str = username.c_str();
					strcpy(send_msg.txt_username, str);
					str = text.c_str();
					strcpy(send_msg.txt_text, str);
					//send_msg.txt_username, *username.c_str();
					//send_msg.txt_text,*text.c_str();
					send_data = &send_msg;

					len = sizeof send_msg;

					//cout << username <<endl;
					struct sockaddr_in send_sock = channel_user_iter->second;


					//bytes = sendto(s, send_data, len, 0, (struct sockaddr*)&send_sock, fromlen);
					bytes = sendto(s, send_data, len, 0, (struct sockaddr*)&send_sock, sizeof send_sock);

					if (bytes < 0){
						perror("Message failed\n"); //error
					}
					else{
						//printf("Message sent\n");
					}
				}
				cout << "server: " << username << " sends say message in " << channel << endl;

				// servers use their random number generator to create unique identifiers
				// https://stackoverflow.com/questions/35726331/c-extracting-random-numbers-from-dev-urandom
				unsigned long long int id = 0; //Declare value to store data into
			    size_t size = sizeof(id); //Declare size of data
			    ifstream urandom("/dev/urandom", ios::in|ios::binary); //Open stream
			    if (urandom) { // stream is open
			    	urandom.read(reinterpret_cast<char*>(&id), size);
			    	if (!urandom){
			    		cout << "Can't read value from /dev/urandom" << endl;
			    	}
			    	// append id to list so and close file
			    	printf("DEBUG: adding id %llu to vector\n", id);
			    	users_id.push_back(id);
			    	urandom.close();
			    }else{ 
			    	cout << "Failed to open /dev/urandom" << endl;
			    }

			    // now we need to send msg to all the users in "channel"
			    map<string,struct sockaddr_in> channel_users = channels[channel].channel_type;
			    map<string,struct sockaddr_in>::iterator sock_iter;
			    for (sock_iter = channel_users.begin(); sock_iter != channel_users.end(); sock_iter++){
			    	struct sockaddr_in send_sock = channel_user_iter->second;
			    	send_say(channel, username, text, id, send_sock);
			    }
			}
		}
	}
}

void send_say(string channel, string username, string text, unsigned long long int id, struct sockaddr_in send_sock){
	ssize_t bytes;
	void *send_data;
	size_t len;

	struct request_ss_say send_msg;
	send_msg.req_type = TXT_SAY;

	const char* str = channel.c_str();
	strcpy(send_msg.txt_channel, str);
	str = username.c_str();
	strcpy(send_msg.txt_username, str);
	str = text.c_str();
	strcpy(send_msg.txt_text, str);
	send_msg.req_uid = id;
	//send_msg.txt_username, *username.c_str();
	//send_msg.txt_text,*text.c_str();
	send_data = &send_msg;

	len = sizeof send_msg;

	//cout << username <<endl;
	


	//bytes = sendto(s, send_data, len, 0, (struct sockaddr*)&send_sock, fromlen);
	bytes = sendto(s, send_data, len, 0, (struct sockaddr*)&send_sock, sizeof send_sock);

	if (bytes < 0){
		perror("Message failed\n"); //error
	}
	else{
		//printf("Message sent\n");
	}
}


void handle_list_message(struct sockaddr_in sock){

	//check whether the user is in usernames
	//if yes, send a list of channels
	//if not send an error message to the user



	string ip = inet_ntoa(sock.sin_addr);

	int port = sock.sin_port;

 	char port_str[6];
 	sprintf(port_str, "%d", port);
	string key = ip + "." +port_str;


	//check whether key is in rev_usernames
	map <string,string> :: iterator iter;


	iter = rev_usernames.find(key);
	if (iter == rev_usernames.end() ){
		//ip+port not recognized - send an error message
		send_error_message(sock, "Not logged in ");
	}
	else{
		string username = rev_usernames[key];
		int size = channels.size();
		//cout << "size: " << size << endl;

		active_usernames[username] = 1;

		ssize_t bytes;
		void *send_data;
		size_t len;


		//struct text_list temp;
		struct text_list *send_msg = (struct text_list*)malloc(sizeof (struct text_list) + (size * sizeof(struct channel_info)));


		send_msg->txt_type = TXT_LIST;

		send_msg->txt_nchannels = size;


		map<string,struct users>::iterator channel_iter;



		//struct channel_info current_channels[size];
		//send_msg.txt_channels = new struct channel_info[size];
		int pos = 0;

		for(channel_iter = channels.begin(); channel_iter != channels.end(); channel_iter++){
			string current_channel = channel_iter->first;
			const char* str = current_channel.c_str();
			//strcpy(current_channels[pos].ch_channel, str);
			//cout << "channel " << str <<endl;
			strcpy(((send_msg->txt_channels)+pos)->ch_channel, str);
			//strcpy(((send_msg->txt_channels)+pos)->ch_channel, "hello");
			//cout << ((send_msg->txt_channels)+pos)->ch_channel << endl;

			pos++;

		}



		//send_msg.txt_channels =
		//send_msg.txt_channels = current_channels;
		send_data = send_msg;
		len = sizeof (struct text_list) + (size * sizeof(struct channel_info));

					//cout << username <<endl;
		struct sockaddr_in send_sock = sock;


		//bytes = sendto(s, send_data, len, 0, (struct sockaddr*)&send_sock, fromlen);
		bytes = sendto(s, send_data, len, 0, (struct sockaddr*)&send_sock, sizeof send_sock);

		if (bytes < 0){
			perror("Message failed\n"); //error
		}
		else{
			//printf("Message sent\n");

		}

		cout << "server: " << username << " lists channels"<<endl;


	}



}


void handle_who_message(void *data, struct sockaddr_in sock){


	//check whether the user is in usernames
	//if yes check whether channel is in channels
	//if yes, send user list in the channel
	//if not send an error message to the user


	//get message fields
	struct request_who* msg;
	msg = (struct request_who*)data;

	string channel = msg->req_channel;

	string ip = inet_ntoa(sock.sin_addr);

	int port = sock.sin_port;

 	char port_str[6];
 	sprintf(port_str, "%d", port);
	string key = ip + "." +port_str;


	//check whether key is in rev_usernames
	map <string,string> :: iterator iter;


	iter = rev_usernames.find(key);
	if (iter == rev_usernames.end() ){
		//ip+port not recognized - send an error message
		send_error_message(sock, "Not logged in ");
	}
	else{
		string username = rev_usernames[key];

		active_usernames[username] = 1;

		map<string,struct users>::iterator channel_iter;

		channel_iter = channels.find(channel);

		if (channel_iter == channels.end()){
			//channel not found
			send_error_message(sock, "No channel by the name " + channel);
			cout << "server: " << username << " trying to list users in non-existing channel " << channel << endl;

		}
		else{
			//channel exits
			map<string,struct sockaddr_in> existing_channel_users;
			existing_channel_users = channels[channel].channel_type;
			int size = existing_channel_users.size();

			ssize_t bytes;
			void *send_data;
			size_t len;


			//struct text_list temp;
			struct text_who *send_msg = (struct text_who*)malloc(sizeof (struct text_who) + (size * sizeof(struct user_info)));


			send_msg->txt_type = TXT_WHO;

			send_msg->txt_nusernames = size;

			const char* str = channel.c_str();

			strcpy(send_msg->txt_channel, str);



			map<string,struct sockaddr_in>::iterator channel_user_iter;

			int pos = 0;

			for(channel_user_iter = existing_channel_users.begin(); channel_user_iter != existing_channel_users.end(); channel_user_iter++){
				string username = channel_user_iter->first;

				str = username.c_str();

				strcpy(((send_msg->txt_users)+pos)->us_username, str);


				pos++;



			}

			send_data = send_msg;
			len = sizeof(struct text_who) + (size * sizeof(struct user_info));

						//cout << username <<endl;
			struct sockaddr_in send_sock = sock;


			//bytes = sendto(s, send_data, len, 0, (struct sockaddr*)&send_sock, fromlen);
			bytes = sendto(s, send_data, len, 0, (struct sockaddr*)&send_sock, sizeof send_sock);

			if (bytes < 0){
				perror("Message failed\n"); //error
			}
			else{
				//printf("Message sent\n");
			}
			cout << "server: " << username << " lists users in channnel "<< channel << endl;
		}
	}
}



void send_error_message(struct sockaddr_in sock, string error_msg){
	ssize_t bytes;
	void *send_data;
	size_t len;

	struct text_error send_msg;
	send_msg.txt_type = TXT_ERROR;

	const char* str = error_msg.c_str();
	strcpy(send_msg.txt_error, str);

	send_data = &send_msg;

	len = sizeof send_msg;


	struct sockaddr_in send_sock = sock;



	bytes = sendto(s, send_data, len, 0, (struct sockaddr*)&send_sock, sizeof send_sock);

	if (bytes < 0){
		perror("Message failed\n"); //error
	}
	else{
		//printf("Message sent\n");
	}
}