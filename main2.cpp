#include "socket.hpp"

int main()
{
    Socket* me = new Socket();
    Socket* them = new Socket();

    // loop and listen for packets
    int i = 0;
    while (true)
    {
        // listen for packets
        std::string msg = "Message " + std::to_string(i++) + "\n";
        me->send(them, msg);

        // print the message
        std::cout << "Sent message" << std::endl;

        usleep(1000000);
    }
}