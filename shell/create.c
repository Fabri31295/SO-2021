/* 
 * Sistemas operativos 2021 
 * Almaraz Fabricio, Buñes Juan.
 * 
 * */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio_ext.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <sys/wait.h>
#include "shared.h"

void crear_archivo(char *name) {
	if(creat(name, 0777) == -1)
		perror("No se pudo crear el archivo: ");	
	print_path();
}

int main(int argc, char *argv[]) {
	crear_archivo(argv[1]);
	return 0;
}
