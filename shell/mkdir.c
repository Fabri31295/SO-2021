/* 
 * Sistemas operativos 2021 
 * Almaraz Fabricio, Buñes Juan.
 * 
 * */
#include "shared.h"

// Crea una nueva carpeta con el nombre pasado por parametro
void create_directory(char *name) {
	if(mkdir(name,0777) != 0)
		perror("No se pudo crear el directorio");
	print_path();
}

int main(int argc, char *argv[]) {
	create_directory(argv[1]);
	return 0;
}
