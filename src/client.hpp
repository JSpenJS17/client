#include "client_socket.hpp"
#ifndef CLIENT_HPP
#define CLIENT_HPP

extern ClientSocket* clientSocket;
int check_error(string wherefrom);
void sender();
void listener();

#endif