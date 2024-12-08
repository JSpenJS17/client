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
#include "socket.hpp"

#define MAXLINE 1024 

Socket::Socket(in_addr_t ip, in_port_t port)
{
	// initialize server with given IP
	if (my_sockfd = socket(AF_INET, SOCK_DGRAM, 0) < 0)
	{ 
		fprintf(stderr, "socket creation error");
		exit(EXIT_FAILURE); 
	} 

	// Set socket defaults
	memset(&my_socket, 0, sizeof(my_socket)); 
	my_socket.sin_family = AF_INET; // IPv4 

	// bind the socket
	bind(my_sockfd, (const struct sockaddr *)&my_socket, sizeof(my_socket));
}

Socket::Socket()
{
	// Default constructor, IP is all and port is 8080
	in_addr_t ip = 0;
	in_port_t port = 8080;

	// initialize server with given IP (default all) and port (default 8080 and bind it
	if (my_sockfd = socket(AF_INET, SOCK_DGRAM, 0) < 0)
	{ 
		fprintf(stderr, "socket creation error");
		exit(EXIT_FAILURE); 
	} 

	// Set socket defaults
	memset(&my_socket, 0, sizeof(my_socket)); 
	my_socket.sin_family = AF_INET; // IPv4 

	// bind the socket
	bind(my_sockfd, (const struct sockaddr *)&my_socket, sizeof(my_socket));
}

int Socket::set_ip(in_addr_t new_ip)
{
	throw new ToDoError();
}

int Socket::set_port(in_addr_t in_port)
{
	throw new ToDoError();
}

sockaddr_in* Socket::get_sockaddr() 
{
	// returns a pointer to my sockaddr_in.
	// not meant for the end user to use, might want to find a way to access
	// this from within another Socket without exposing it to the end user.
	return &my_socket;
}

std::string Socket::listen(Socket* other_guy)
{
	// Given another Socket object, will try to receive from them.
	
	// Grab their *real* socket
	sockaddr_in* their_socket = other_guy->get_sockaddr();

	// set our inaddr and inport to their addr and port
	my_socket.sin_addr.s_addr = their_socket->sin_addr.s_addr; 
	my_socket.sin_port = htons(their_socket->sin_port);

	socklen_t len = 0;

	// block and wait for someone to respond!
	int n = recvfrom(my_sockfd, buf, MAXLINE, 
						MSG_WAITALL, (struct sockaddr *) other_guy, &len);

	// return the message
	return std::string(buf);
}

void Socket::send(Socket* other_guy, std::string msg)
{
	// throw a packet to the given Socket
	sockaddr_in* their_socket = other_guy->get_sockaddr();

	// set our inaddr and inport to their addr and port
	my_socket.sin_addr.s_addr = their_socket->sin_addr.s_addr;
	my_socket.sin_port = htons(their_socket->sin_port);

	// send the message
	sendto(my_sockfd, (const char*) msg.c_str(), msg.length(), 
			MSG_CONFIRM, (const struct sockaddr *) other_guy, 
			sizeof(*other_guy));

	return;	
}

// overload == operator
bool Socket::operator==(const Socket &rhs)
{
	return (my_ip == rhs.my_ip && my_port == rhs.my_port);
}

// #ifndef main
// // Driver code 
// int main() 
// { 
// 	int sockfd; 
// 	char buffer[MAXLINE]; 
// 	const char *hello = "Hello from server"; 
// 	struct sockaddr_in servaddr, cliaddr;
	
// 	// Bind the socket with the server address 
// 	if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) 
// 	{ 
//         fprintf(stderr, "bind error");
// 		exit(EXIT_FAILURE); 
// 	} 
	
// 	socklen_t len;
//     int n; 

// 	len = sizeof(cliaddr); //len is value/result 

//     // infinite loop for chat
//     while (true) {
//         n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
//                     MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
//                     &len); 
//         buffer[n] = '\0'; 
//         printf("Client : %s\n", buffer); 
//         sendto(sockfd, (const char *)hello, strlen(hello), 
//             MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
//                 len); 
//         printf("Hello message sent");
//     }
	
// 	return 0; 
// }
// #endif