#include "engine.hpp"
#include "client_socket.hpp"
#include <thread>
#include <mutex>

using namespace std;
ClientSocket* clientSocket;

void sigint_handler(int sig)
{
    delete clientSocket;
    #if defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
    reset_termios();
    #endif
    exit(0);
}

void check_error()
{
    if (errno) {
        fprintf(stderr, "Error Number: %d\n", errno);
        fprintf(stderr, "Error: %s\n", strerror(errno));
        sigint_handler(0);
        exit(1);
    }
}

void input_sender() {
    while (true) {
        // block until we get an input
        char key = wait_for_kb_input();
        // null terminate it so we know when it ends
        char key_send[2] = {key, '\0'};

        // send it!
        clientSocket->send_msg(key_send);
        check_error();
    }
}

int main()
{
    #if defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
    init_termios();
    #endif

    // creating socket
    cout << "Creating socket..." << endl;
    clientSocket = new ClientSocket();
    check_error();
    
    // specifying address
    const char* server_ip = "131.186.7.78";
    int port = 5000;

    // sending connection request
    cout << "Connecting to server at " << server_ip << ":" << port << "..." << endl;
    clientSocket->connect_to(server_ip, port);
    check_error();

    // sending data
    cout << "Connected!" << endl;

    // create a thread that listens for input and sends it
    thread sender(input_sender);

    while (true) {
        // wait to receive input
        char receive_buffer[1024];
        clientSocket->receive_msg(receive_buffer, sizeof(receive_buffer));
        check_error();

        // if we got any
        if (strlen(receive_buffer) > 0) 
        {
            cout << "Received: " <<  receive_buffer << endl;
        }

        // flush stdin to avoid input queueing
        fflush(stdin);
    }

    // closing socket
    sender.join();
    cout << "Closing socket..." << endl;
    delete clientSocket;

    #if defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
    reset_termios();
    #endif

    return 0;
}