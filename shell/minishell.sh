#!/bin/bash
clear
gcc -Wall minishell.c -o ejecutable
gcc -Wall rmdir.c -o rmdir
gcc -Wall mkdir.c -o mkdir
gcc -Wall create.c -o create
gcc -Wall show.c -o show
gcc -Wall chperm.c -o chperm
gcc -Wall help.c -o help
gcc -Wall dirlist.c -o dirlist
gcc -Wall quit.c -o quit
./ejecutable
