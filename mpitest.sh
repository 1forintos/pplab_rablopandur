#!/bin/bash

cleanup() {
    kill -9 $BPID $CPID &>/dev/null
}

trap "cleanup" EXIT

if [ x$RABLO_PORT = x ]; then 
   let RABLO_PORT=15000+`id -u`
fi

PORT=$RABLO_PORT
HOST=localhost

echo RABLOr_PORT: $RABLO_PORT

echo "A gonosz rablo megjelent a banknal es kirabolta."
./main $HOST $PORT burglar &>burglar.log &
BPID=$!
echo "A rablo elkezdett menekulni."
sleep 5
mpirun -np 5 mpimain $HOST $PORT &>cops.log &
CPID=$!
echo "A rendorok elindultak."
sleep 30

echo "Sikerult e elfogni a rablot vagy sem, lejart az ido!"

