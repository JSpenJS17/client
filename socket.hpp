#ifndef SOCKET_HPP
#define SOCKET_HPP

// Server side implementation of UDP client-server model 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <stdio.h>
#include <string.h>

// todo errors
#include "ToDoError.cpp"

#define MAXLINE 1024 

class Socket {
public:
    Socket(in_addr_t ip, in_port_t port);

    Socket();

	int set_ip(in_addr_t new_ip);

	int set_port(in_addr_t in_port);

	sockaddr_in* get_sockaddr();

	std::string listen(Socket* other_guy);

	void send(Socket* other_guy, std::string msg);

	// overload == operator
	bool operator==(const Socket &rhs);

private:
	// my ip/port
	in_addr_t my_ip;
	in_port_t my_port;

	// my socket object, not settable, only generatable
	sockaddr_in my_socket;

	// my socket file descriptor, also not settable
	int my_sockfd;
	
	// buffer for holding incoming packets
	char buf[MAXLINE];
};

#endif