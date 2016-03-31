#ifndef PLAYER_H
#define PLAYER_H

#include <stdexcept>

class Player
{
    public:
        typedef enum {up, down, left, right, none} direction;
        typedef enum {burglar, cop} player_t;

        // Create player and login to the server
        Player(const char *host, unsigned int port, player_t cop);

        // Move on the board
        char *move(direction d);

        // getters
        int getx(){ return x; }
        int gety(){ return y; }
        player_t gettyp() { return me; }

        // setters
        void setx(int x) { this->x = x; }
        void sety(int y) { this->y = y; }
    private:
        int x;                                    // x pozition
        int y;                                    // y pozition
        int s;                                    // socket
        player_t me;                              // type
};
#endif
