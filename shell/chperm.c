/* 
 * Sistemas operativos 2021 
 * Almaraz Fabricio, Buñes Juan.
 * 
 * */
#include "shared.h"

void cambiar_permisos(char *name, char*perms) {
	char *pi;
	int permisos;
	errno = 0; // variable para el manejo de errores
	long conv = strtol(perms, &pi, 8); // convierte el String perms a un entero LONG en base octal
	
	if(errno != 0 || *pi != '\0' || conv > INT_MAX) {
		printf("Valor para el parametro permisos no valido\n");
	}
	permisos = conv;

	if(chmod(name,permisos) != 0)
		perror("Error al cambiar los permisos");
	print_path();
}

int main(int argc, char *argv[]) {
	cambiar_permisos(argv[1],argv[2]);
	return 0;
}
	
