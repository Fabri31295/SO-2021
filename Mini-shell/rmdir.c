/* 
 * Sistemas operativos 2021 
 * Almaraz Fabricio, Buñes Juan.
 * 
 * */
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
