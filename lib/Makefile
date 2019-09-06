CC = gcc
CFLAGS = -Wall
LDFLAGS = -L ./ -I ./
.PHONY: clean

all: libFunzioni.a  clientLib/clientLib.o common/common.o IO/io.o MyHash/hash.o serverLib/serverLib.o socket/socket.o 

libFunzioni.a : clientLib/clientLib.o common/common.o IO/io.o MyHash/hash.o serverLib/serverLib.o socket/socket.o 
	ar rvs $@ $^
	-rm $^

socket/socket.o : socket/socket.c
	$(CC) $^ $(CGLAGS) $(LDFLAGS) -c -o $@

IO/io.o : IO/io.c
	$(CC) $^ $(CGLAGS) $(LDFLAGS) -c -o $@

MyHash/hash.o : MyHash/hash.c
	$(CC) $^ $(CGLAGS) $(LDFLAGS) -c -o $@

clientLib/clientLib.o : clientLib/clientLib.c
	$(CC) $^ $(CGLAGS) $(LDFLAGS) -c -o $@

serverLib/serverLib.o : serverLib/serverLib.c
	$(CC) $^ $(CGLAGS) $(LDFLAGS) -c -o $@

common/common.o : common/common.c
	$(CC) $^ $(CGLAGS) $(LDFLAGS) -c -o $@
	

