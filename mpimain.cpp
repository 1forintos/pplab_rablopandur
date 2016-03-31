//
// MPI kommunikacios pelda
//
// Buta redndor, aki ossze-vissya rohangal es felkialt,
// ha rablot lat. Ezt a tobbiek halljak, de nem tudjak
// hol a rablo, mert azt nem mondja meg az ostobaja.
// Forditas:
//  mpiCC -o mpimain mpimain.cpp player.cpp
// 
// Futtatas: (lam inditasa es egy rablo felpakolasa utan)
//  mpirun -np 3 mpimain localhost 15623
//
//
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <mpi.h>

#include "player.h"

using namespace std;

#define TAG_RABLO 1

int main(int argc, char ** argv)
{
    try {
        MPI::Init(argc, argv);
        int rank = MPI::COMM_WORLD.Get_rank();
        int size = MPI::COMM_WORLD.Get_size();
    
        if(argc != 3) {
            cerr << "Usage: " << argv[0] << " <hostname> <port>" << endl;
            MPI::Finalize();
            return 1;;
        }
    
        // argumentumok feldolgozasa
        stringstream ss(argv[2]);
        int port;
        ss >> port;
    
        // rendor inditasa
        Player pl(argv[1], port, Player::cop);
        cerr << rank << ". rendor kezdo pozicioja: " << pl.getx() << "," << pl.gety() << endl;    
        Player::direction d = Player::none;

        while(1) {
            struct timespec tim = { 0, 100000 }, trem;
            nanosleep(&tim, &trem);
            MPI::Status stat;
            if (MPI::COMM_WORLD.Iprobe(MPI::ANY_SOURCE, TAG_RABLO, stat)) { // jott valami
                char buff[6];
                MPI::COMM_WORLD.Recv(buff, 6, MPI::CHAR, stat.Get_source(), TAG_RABLO);
                cerr << rank << ". rendor uzenetet kapott (" << stat.Get_source() << ". rendortol): " << buff << endl;
            }
    
            int r = rand() % (Player::right + 1);
            char *buffer;
            if(d == Player::none) {
                if (r == 0) d = Player::up;
                else if (r==1) d = Player::down;
                else if (r==2) d = Player::left;
                else d = Player::right;    
                        
            }
            buffer = pl.move(d);    
            d = Player::none;
            Player::direction processState;
            for (char *p = buffer; *p; p++) {
                switch(*p) {
                    case 'U':
                        processState = Player::up;
                    break;
                    case 'R':
                        processState = Player::right;
                    break;
                    case 'D':
                        processState = Player::down;
                    break;
                    case 'L':
                        processState = Player::left;
                    break;
                    default:
                        if (*p == '1') {  
                            d = processState;
                            /*cout << std::endl << "TEST: " << buffer << " END_TEST" << std::endl;                 // rablot latok nagyot kialtok!
                            for (int i = 0; i < size; i++) {  // minden rendornek elkuldjuk
                                if (i != rank)
                                    MPI::COMM_WORLD.Send("Rablo", 6, MPI::CHAR, i, TAG_RABLO);
                            }*/


                            //break;                            // csak egy rablora szamitunk
                        }  
                        break;
                }
            }
            delete[] buffer;
        }
        MPI::Finalize();
    } catch (runtime_error &err) {
	cerr << err.what() << endl;
	MPI::COMM_WORLD.Abort(1);
    } catch (MPI::Exception &err) {	// ez altalaban nincs enedelyezve ....
	cerr << err.Get_error_string() << endl;
        MPI::COMM_WORLD.Abort(1);
    }
    return 0;
}
