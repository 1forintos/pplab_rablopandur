#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include "player.h"

using namespace std;

#ifndef MSG_NOSIGNAL
# define MSG_NOSIGNAL 0
#endif

Player::Player(const char *host, unsigned int port, player_t cop) {
    s = socket(PF_INET, SOCK_STREAM, 0);
    if (s < 0) 
        throw runtime_error("Couldn't open socket!");
    struct hostent *hostinfo;
    struct sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = htons (port);
    hostinfo = gethostbyname (host);
    if (hostinfo == NULL) 
        throw runtime_error("No nhostinfo!");
    name.sin_addr = *(struct in_addr *) hostinfo->h_addr;
    if (connect(s, (struct sockaddr *) &name, sizeof (name)) < 0) {
	stringstream err;
        err << "Can't connect to server " << host << " through port " << port;
        throw runtime_error(err.str());
    }

    //send login message
    me = cop;
    char login[3] = "M\n";
    if (cop) login[0] = 'N';
    if(send(s, login, 2, MSG_NOSIGNAL) <= 0) 
        throw runtime_error("Unable to send");
    cerr << "Login message sent" << endl;
    //wait for initial pozition
    char buffer[128];
    int poz = 0;
    do {
        if (recv(s, &buffer[poz], 1, 0) <= 0) 
            throw runtime_error("Recv error");
        cerr << buffer[poz];
    } while(buffer[poz++] != '\n');
    buffer[poz] = '\0';
    stringstream ss;
    ss << buffer;
    ss >> x;
    ss.ignore(1);
    ss >> y;
}


char *Player::move(direction d) {
    switch (d) {
        case up:
            if (send(s, "U", 1, MSG_NOSIGNAL) <= 0) throw runtime_error("Move: send error");
            break;
        case down:
            if (send(s, "D", 1, MSG_NOSIGNAL) <= 0) throw runtime_error("Move: send error");
            break;
        case left:
            if (send(s, "L", 1, MSG_NOSIGNAL) <= 0) throw runtime_error("Move: send error");
            break;
        case right:
            if (send(s, "R", 1, MSG_NOSIGNAL) <= 0) throw runtime_error("Move: send error");
            break;
        default:
            throw runtime_error("Illegal move cmd");
    }
    char * buffer = new char[128];
    int poz = 0;
    do {
        if (recv(s, &buffer[poz], 1, 0) <= 0) 
            throw runtime_error("Move: recv error");
    } while (buffer[poz++] != '\n');
    buffer[poz]='\0';
    return buffer;
}
