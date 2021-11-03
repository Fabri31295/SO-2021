/* 
 * Sistemas operativos 2021 
 * Almaraz Fabricio, Buñes Juan.
 * 
 * */
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
