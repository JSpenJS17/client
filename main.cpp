#include "socket.hpp"

int main()
{
    Socket* me = new Socket();
    Socket* them = new Socket();

    // loop and listen for packets
    while (true)
    {
        // listen for packets
        std::string msg = me->listen(them);

        // print the message
        std::cout << msg;
    }
}