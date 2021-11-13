#! /bin/bash
gcc $1 net/tcp_socket.c -o tmp/$1.out && ./tmp/$1.out