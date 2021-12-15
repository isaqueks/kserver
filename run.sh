#! /bin/bash
gcc $1 $(ls source/*/*.c) -o tmp/$1.out && ./tmp/$1.out