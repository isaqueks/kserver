#! /bin/bash
gcc main.c $(ls source/*/*.c) -o main.out && ./main.out