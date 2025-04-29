#include "engine.hpp"
#include "client_socket.hpp"
#include <thread>
#include <mutex>
#include <signal.h>

#define BUFFER_SIZE 1024
#define MAX_PLAYERS 2
#define MAX_X 15
#define MAX_Y 15
#define MIN_X 0
#define MIN_Y 0

using namespace std;

Pixel bg_pixel(' ', BLACK, BLACK);

class Player {
    public:
        Player(int id, int x, int y, Pixel pixel)
            : id(id), x(x), y(y), pixel(pixel) {}
        
        int id;
        int x;
        int y;
        Pixel pixel;

        void move(char direction) {
            switch (direction) {
                case 'w':
                    if (y > MIN_Y) y--;
                    break;
                
                case 'a':
                    if (x > MIN_X) x--;
                    break;

                case 's':
                    if (y < MAX_Y-1) y++;
                    break;

                case 'd':
                    if (x < MAX_X-1) x++;
                    break;
            }
        }

};

Player players[MAX_PLAYERS] = { 
    Player(0, 0, 0, Pixel('@', BLUE, BLUE)), 
    Player(1, 0, 0, Pixel('@', RED, RED)) 
  };

class GameBoard {
    public:
        GameBoard(int width, int height) :
            width(width), height(height) {
            board = new Board(width, height, bg_pixel);
        }

        ~GameBoard() {
            delete board;
        }

        void board_updater(int p_num, char inp) {
            board_mutex.lock();

            // Place players on the board
            board->write(players[1].y, players[1].x, bg_pixel);
            board->write(players[0].y, players[0].x, bg_pixel);

            // Update player position based on input
            players[p_num].move(inp);

            // Place players on the board
            board->write(players[1].y, players[1].x, players[1].pixel);
            board->write(players[0].y, players[0].x, players[0].pixel);

            // Draw the updated board
            board->draw(0, false);
            fflush(stdout);

            board_mutex.unlock();
        }

        void draw() {
            board->draw(0, false);
            fflush(stdout);
        }

        void write(int row, int col, Pixel pixel) {
            board->write(row, col, pixel);
        }

        int width;
        int height;
        Board* board;
    
    private:
        mutex board_mutex;
};

GameBoard* game;

ClientSocket* clientSocket;
string starting_string = "Hello from client";

void reset_screen() 
{
	clear_screen();
	show_cursor(true);
	color(16, 16);
    #if defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
    reset_termios();
    #endif
}

void sigint_handler(int sig)
{
	reset_screen();
    delete clientSocket;
    delete game;
    exit(0);
}

void check_error(string wherefrom)
{
    if (errno) {
        reset_screen();
        fflush(stdout);
        cout << "Crash! From " << wherefrom << endl;
        cout << "Error Number: " << errno << endl;
        cout << "Error: " << strerror(errno) << endl;
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

			if (receive_buffer[i] == -1) {
				// other client disconnected
                    // lowkey this should be more robust and actually do stuff
                    // like close the socket
				reset_screen();
				cout << "Partner disconnected." << endl;
				exit(1);
			}
            // Update the board
            game->board_updater(1, receive_buffer[i]);
        }

        // fflush(stdin);
        // delay(16);
    }
}

int main()
{
    #if defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
    init_termios();
    #endif
    signal(SIGINT, sigint_handler);

    // creating socket
    cout << "Creating socket..." << endl;
    clientSocket = new ClientSocket();
    check_error("main() -- create socket");
    
    // specifying address
    const char* server_ip = "131.186.7.78";
    int port = 5000;

    // sending connection request
    cout << "Connecting to server at " << server_ip << ":" << port << "..." << endl;
    clientSocket->connect_to(server_ip, port);
    check_error("main() -- connect to server");

    // sending data
    cout << "Connected!" << endl;

    clear_screen();
    show_cursor(false);

    // Create game board
    game = new GameBoard(MAX_X, MAX_Y);
    game->board_updater(0, ' '); // Initialize board with players
    game->draw();
    
    try {
        // Start sender and listener in separate threads
        thread sender_thread(sender);
        thread listener_thread(listener);

        // Wait for both threads to finish (they won't, as these are infinite loops)
        sender_thread.join();
        listener_thread.join();
    } catch (const exception& ex) {
        reset_screen();
        cerr << "Thread error: " << ex.what() << endl;
        check_error("main() -- thread error");
        exit(1);
    }

    // closing socket
    cout << "Closing socket..." << endl;
    delete clientSocket;
    delete game;

    #if defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
    reset_termios();
    #endif

    return 0;
}
