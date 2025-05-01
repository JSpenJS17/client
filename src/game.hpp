#ifndef GAME_HPP
#define GAME_HPP

#define MAX_X 15
#define MAX_Y 15
#define MIN_X 0
#define MIN_Y 0
#define BUFFER_SIZE 1024
#define MAX_PLAYERS 2

// Includes
#include <thread>
#include <mutex>
#include <signal.h>
#include "lib/engine.hpp"
#include "client.hpp"
#include "player.hpp"

using namespace std;

void reset_screen();

class GameBoard {
    public:
        GameBoard(int width, int height);

        ~GameBoard() {
            delete board;
        }

        void board_updater(int p_num, char inp);

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

extern Player players[MAX_PLAYERS];
extern Pixel bg_pixel;

// The one and only GameBoard object
extern GameBoard* game;

#endif