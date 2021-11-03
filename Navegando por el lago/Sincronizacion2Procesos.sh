#!/bin/bash
clear
gcc Sincronizacion2procesos.c -o Sincronizacion2ProcesosEjecutable -lpthread
gcc primera.c -o primera -lpthread
gcc business.c -o business -lpthread
gcc turista.c -o turista -lpthread
chmod +x Sincronizacion2Procesos.sh
./Sincronizacion2ProcesosEjecutable

