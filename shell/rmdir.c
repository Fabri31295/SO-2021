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

void remove_directory(char *name) {
	if(rmdir(name) != 0)
		perror("Error al eliminar el directorio");
	print_path();
}

int main(int argc, char *argv[]) {
	remove_directory(argv[1]);
	return 0;
}
