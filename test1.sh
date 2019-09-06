 #!bin/bash


exec &> testout.log
serverPid=$(pidof ./bin/server.o)
 for i in {1..50}
do
	./bin/client.o Test_$i 1 &
	pids[$i]=$!
done

for pid in ${pids[*]}; do
	wait $pid
done

kill -SIGUSR1 $serverPid
for i in {1..30}
do
    ./bin/client.o Test_$i 2 &
	pids[$i]=$!
done

for pid in ${pids[*]}; do
	wait $pid
done
kill -SIGUSR1 $serverPid

for i in {31..50}
do
    ./bin/client.o Test_$i 3 &
	pids[$i]=$!
done

for pid in ${pids[*]}; do
	wait $pid
done
kill -SIGUSR1 $serverPid

sleep 2 && kill -SIGTERM $serverPid

