#!bin/bash
rm -r data
rm ./bin/server.o ./bin/client.o
cd lib && make && cd ..
make
clear 
