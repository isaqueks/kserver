#! /bin/bash

mkdir tmp

gcc tests/$1.c net/tcp_socket.c -o tmp/$1.out
tmp/$1.out && echo "Test passed."