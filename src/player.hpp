// #include "game.hpp"
// #ifndef PLAYER_HPP
// #define PLAYER_HPP

// class Player {
//     public:
//         Player(){}

//         Player(int id, int x, int y, Pixel pixel)
//             : id(id), x(x), y(y), pixel(pixel) {}
        
//         int id;
//         int x;
//         int y;
//         Pixel pixel;

//         void move(char direction) {
//             switch (direction) {
//                 case 'w':
//                     if (y > MIN_Y) y--;
//                     break;
                
//                 case 'a':
//                     if (x > MIN_X) x--;
//                     break;

//                 case 's':
//                     if (y < MAX_Y-1) y++;
//                     break;

//                 case 'd':
//                     if (x < MAX_X-1) x++;
//                     break;
//             }
//         }

// };

// #endif