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

void show_directory(char *route) {
	DIR *d = opendir(route);
	struct dirent *dentry;
	int i = 1;
	if(d == NULL) {
		printf("No se puede abrir la ruta [%s]\n",route);
	}
	else {
		printf("Contenido de la direccion %s:\n",route);
		while((dentry = readdir(d)) != NULL) {
			printf("%d. %s\n", i, dentry -> d_name);
			i++;
		}
	}
	closedir(d);
	print_path();
}

int main() {
	show_directory(int argv[1]);
	return 0;
}

