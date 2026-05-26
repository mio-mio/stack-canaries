#!/bin/bash

gcc -g -O0 -fno-stack-protector canary_demo.c -o no_canary
gcc -g -O0 -fstack-protector canary_demo.c -o sp
gcc -g -O0 -fstack-protector-strong canary_demo.c -o sp_strong
gcc -g -O0 -fstack-protector-all canary_demo.c -o sp_all
gcc -g -O0 -fstack-protector-explicit canary_demo.c -o sp_explicit
