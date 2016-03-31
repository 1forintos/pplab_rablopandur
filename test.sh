#!/bin/bash

cleanup() {
    kill -9 $BPID $CPIDS &>/dev/null
}

trap "cleanup" EXIT

if [ x$RABLO_PORT = x ]; then 
   let RABLO_PORT=15000+`id -u`
fi

echo RABLO_PORT: $RABLO_PORT
PORT=$RABLO_PORT
HOST=localhost
COPS=5

echo "A gonosz rablo megjelent a banknal es kirabolta."
./main $HOST $PORT burglar &>burglar.log &
BPID=$!
echo "A rablo elkezdett menekulni."
sleep 5

echo "A rendorok kivonulnak a helyszinre."
for i in `seq 1 $COPS`
do
	./main $HOST $PORT cop &>cop.$i.log &
	echo "Kicammogott a(z) $i. rendor."
	CPIDS="$CPIDS $!"
	sleep 1
done

echo "Mar nem jon tobb rendor a rendorseg eroforrasai kimerultek."
sleep 30
echo "Sikerult e elfogni a rablot vagy semm, lejart az ido!"
