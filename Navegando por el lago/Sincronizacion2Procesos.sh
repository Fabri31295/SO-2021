#!/bin/bash
clear
gcc Sincronizacion2procesos.c -o Sincronizacion2ProcesosEjecutable
gcc primera.c -o primera 
gcc business.c -o business 
gcc turista.c -o turista 
chmod +x Sincronizacion2Procesos.sh
./Sincronizacion2ProcesosEjecutable

