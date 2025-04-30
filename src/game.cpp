
#include "game.hpp"

GameBoard::GameBoard(int width, int height): width(width), height(height) {
    board = new Board(width, height, bg_pixel);
}

void GameBoard::board_updater(int p_num, char inp) {
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

void reset_screen() 
{
	clear_screen();
	show_cursor(true);
	color(16, 16);
    close_engine();
}

void sigint_handler(int sig)
{
	reset_screen();
    delete clientSocket;
    delete game;
    exit(0);
}

Pixel bg_pixel = Pixel(' ', BLACK, BLACK);
Player players[MAX_PLAYERS] {
        Player(0, 0, 0, Pixel('@', BLUE, BLUE)), 
        Player(1, 0, 0, Pixel('@', RED, RED)) 
};
GameBoard* game = new GameBoard(MAX_X, MAX_Y);

int main()
{
    init_engine();
    signal(SIGINT, sigint_handler);
    
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

    close_engine();

    return 0;
}