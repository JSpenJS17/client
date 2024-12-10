#include "ToDoError.cpp"
#include "client_socket.hpp"

// ClientSocket class implementation
ClientSocket::  ClientSocket() {
    // Constructor
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
}

ClientSocket::~ClientSocket() {
    // Destructor
    close_socket();
}

void ClientSocket::connect_to(const char* server_ip, int port) {
    // takes server IP like so: "xxx.xxx.xxx.xxx"
    // port is just an integer
    // Connect to server
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = inet_addr(server_ip);
    connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
}

void ClientSocket::send_msg(std::string msg) {
    // Send data (std::string) to server
    send_msg(msg.c_str(), msg.size());    
}

void ClientSocket::send_msg(const char* msg, int msg_size) {
    // Send data (null terminated string) to server
    send(clientSocket, msg, msg_size, 0);
}

void ClientSocket::receive_msg(char* buffer, int buffer_size) {
    // Receive data from server
    memset(buffer, 0, buffer_size);
    // do not block
    recv(clientSocket, buffer, buffer_size, 0);
}

void ClientSocket::close_socket() {
    // Close connection without killing the socket object
    close(clientSocket);
}