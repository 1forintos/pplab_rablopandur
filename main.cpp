#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <time.h>

#include "player.h"

using namespace std;

int main(int argc, char ** argv) {
    if(argc != 4) {
        cerr << "Usage: " << argv[0] << " <hostname> <port> burglar|cop>" << endl;
        return 1;;
    }

    // argumentumok feldolgozasa
    stringstream ss(argv[2]);
    int port;
    ss >> port;
    try {
        // jatekos inditasa
        Player pl(argv[1], port, string(argv[3]) == "burglar" ? Player::burglar : Player::cop);
        cerr << argv[3] << ": " << pl.getx() <<" " << pl.gety() << endl;
    
        while(1) {
            struct timespec tim = { 0, 100000 }, trem;
            nanosleep(&tim, &trem);
            int r = rand() % (Player::right + 1);
            Player::direction d;
            if (r == 0) d = Player::up;
            else if (r==1) d = Player::down;
            else if (r==2) d = Player::left;
            else d = Player::right;
            char *buffer = pl.move(d);
#ifdef DEBUG
            cout << "dir:" << r << " typ:" << pl.gettyp() << " ret:" << buffer;	
#endif
            delete[] buffer;
        }
    } catch (runtime_error &e) {
	cerr << e.what() << endl;
    }
    return 0;
}
