#include "game.hpp"
#include "client.hpp"

string starting_string = "Hello from client";
ClientSocket* clientSocket = new ClientSocket();

int check_error(string wherefrom)
{
    if (errno) {
        fflush(stdout);
        cout << "Crash! From " << wherefrom << endl;
        cout << "Error Number: " << errno << endl;
        cout << "Error: " << strerror(errno) << endl;
        return 1;
    }
    return 0;
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
            check_error("sender()");

            // Update the board
            game->board_updater(0, key);
        }

        delay(16);
    }
}

void listener() {
    char receive_buffer[BUFFER_SIZE];
    size_t bytes_received;
    size_t wait_time = 0;
    
    while (true) {
        bytes_received = clientSocket->receive_msg(receive_buffer, sizeof(receive_buffer));
        check_error("listener()");

        for (size_t i = 0; i < bytes_received; i++)
        {
            if (wait_time < starting_string.size())
            {
                // wait through first "Hello from client"
                    // this prevents the dreaded 35 character crash
                wait_time++;
                continue;
            }

			if (receive_buffer[i] < 0) {
				// other client disconnected
                    // lowkey this should be more robust and actually do stuff
                    // like close the socket
				reset_screen();
                cout << "Ack!" << endl;
                cout << "Error: " << get_error_message(receive_buffer[i]) << endl;
				exit(receive_buffer[i]);
			}
            // Update the board
            game->board_updater(1, receive_buffer[i]);
        }
    }
}

