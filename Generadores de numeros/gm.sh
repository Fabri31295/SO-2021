#!/bin/bash
gcc remove.c -o remove
gcc gen_messages.c -o gm
./remove
./gm
