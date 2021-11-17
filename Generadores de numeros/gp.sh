#!/bin/bash
gcc remove.c -o remove
gcc -Wall gen_pipes.c -o gp
./remove
./gp