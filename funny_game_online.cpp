#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

using namespace std;

void check_error() {
    if (errno != 0) {
        cout << "Error Number: " << errno << endl;
        cout << "Error: " << strerror(errno) << endl;
        exit(errno);
    }
}

int main()
{
    int pid = fork();
    if (pid == 0) 
    {
        // creating socket
        int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

        // specifying address
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(8080);
        serverAddress.sin_addr.s_addr = INADDR_ANY;

        // sending connection request
        connect(clientSocket, (struct sockaddr*)&serverAddress,
                sizeof(serverAddress));

        // sending data
        const char* message = "Hello, server!";
        send(clientSocket, message, strlen(message), 0);

        // closing socket
        close(clientSocket);

        return 0;
    }
    else if (pid > 0) 
    {
        // creating socket
        int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

        // specifying the address
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(8080);
        serverAddress.sin_addr.s_addr = INADDR_ANY;

        // binding socket.
        bind(serverSocket, (struct sockaddr*)&serverAddress,
            sizeof(serverAddress));
        check_error();

        // listening to the assigned socket
        listen(serverSocket, 5);
        check_error();

        // accepting connection request
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        check_error();

        // recieving data
        char buffer[1024] = { 0 };
        recv(clientSocket, buffer, sizeof(buffer), 0);
        cout << "Message from client: " << buffer << endl;

        // closing the socket.
        close(serverSocket);

        return 0;
    }
    else
    {
        cout << "Error: Unable to fork." << endl;
        return 1;
    }

    



}