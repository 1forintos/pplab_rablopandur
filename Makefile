#
# Makefile for dummy players
#

SRC = 	main.cpp player.cpp player.h test.sh Makefile mpimain.cpp README
MPICC = mpiCC
CXXFLAGS = -DDEBUG
#LDFLAGS  = -lsocket -lnsl 	# for solaris opsys

all: main mpimain

clean:
	rm -f *.log *.o main mpimain

tar:
	tar -czvf client.tgz ${SRC}

player.o: player.cpp player.h

main.o: main.cpp player.h

main: main.o player.o
	${CXX} ${CXXFLAGS} -o main main.o player.o ${LDFLAGS}

mpimain.o: mpimain.cpp player.h
	${MPICC} ${CXXFLAGS} -c mpimain.cpp

mpimain: mpimain.o player.o
	${MPICC} ${CXXFLAGS} -o mpimain mpimain.o player.o 

