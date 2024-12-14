#include "engine.hpp"
#include "client_socket.hpp"

using namespace std;
ClientSocket* clientSocket;

void sigint_handler(int sig)
{
    delete clientSocket;
    #ifdef LINUX
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

    while (true) {
        char key = wait_for_kb_input();
        
        // send our key
        clientSocket->send_msg(&key);
        check_error();

        // receive input back
        char buffer[1024];
        clientSocket->receive_msg(buffer, 1024);
        check_error();

        cout << "Server response: " << buffer << endl;
        char rps = buffer[0];

        if (rps == 'w') {
            cout << "You win!" << endl;
        } else if (rps == 'l') {
            cout << "You lose!" << endl;
        } else if (rps == 't') {
            cout << "It's a tie!" << endl;
        }

    }

    // closing socket
    cout << "Closing socket..." << endl;
    delete clientSocket;

    #if defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
    reset_termios();
    #endif

    return 0;
}