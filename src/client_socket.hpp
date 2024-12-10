#ifndef CLIENT_SOCKET_HPP
#define CLIENT_SOCKET_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

// ClientSocket class definition
class ClientSocket {
public:
    ClientSocket();
    ~ClientSocket();
    
    void connect_to(const char* server_ip, int port);
    void send_msg(std::string msg);
    void send_msg(const char* msg, int msg_size);
    void receive_msg(char* buffer, int buffer_size);
    void close_socket();
private:
    int clientSocket;
    struct sockaddr_in serverAddress;
};

#endif