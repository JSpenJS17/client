#include "engine.hpp"
#include "client_socket.hpp"
#include <thread>
#include <mutex>

#define BUFFER_SIZE 1024

using namespace std;
ClientSocket* clientSocket;
Board* board;
Pixel bg_pixel(' ', BLACK, BLACK);
string starting_string = "Hello from client";

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

void sender() {
    clientSocket->send_msg(starting_string);
    while (true) {
        // Block until we get an input
        char key = get_kb_input();

        if (key != -1)
        {
            // Send it!
            clientSocket->send_msg(&key, 1);
            check_error();
        }

        // look for input every 25 ms (can be changed)
        delay(16);
    }
}

void listener() {
    char receive_buffer[BUFFER_SIZE];
    size_t bytes_received;
    size_t wait_time = 0;
    
    while (true) {
        bytes_received = clientSocket->receive_msg(receive_buffer, sizeof(receive_buffer));
        check_error();

        for (size_t i = 0; i < bytes_received; i++)
        {
            if (wait_time < starting_string.size())
            {
                wait_time++;
                continue;
            }
            cout << "Received: " << receive_buffer[i] << endl;
        }

        fflush(stdin);
        delay(16);
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

    
    board = new Board(10, 10, bg_pixel);

    try {
        // Start sender and listener in separate threads
        thread sender_thread(sender);
        thread listener_thread(listener);

        // Wait for both threads to finish (they won't, as these are infinite loops)
        sender_thread.join();
        listener_thread.join();
    } catch (const exception& ex) {
        cerr << "Thread error: " << ex.what() << endl;
        check_error();
        exit(1);
    }

    // closing socket
    cout << "Closing socket..." << endl;
    delete clientSocket;

    #if defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
    reset_termios();
    #endif

    return 0;
}