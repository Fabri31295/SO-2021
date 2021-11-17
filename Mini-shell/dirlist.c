/* 
 * Sistemas operativos 2021 
 * Almaraz Fabricio, Buñes Juan.
 * 
 * */
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

int main(int argc, char *argv[]) {
	show_directory(argv[1]);
	return 0;
}

