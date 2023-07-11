#! /bin/bash

mkdir tmp

gcc tests/$1.c $(ls source/*/*.c) -o tmp/$1.out
tmp/$1.out && echo "Test passed."