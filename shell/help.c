/* 
 * Sistemas operativos 2021 
 * Almaraz Fabricio, Buñes Juan.
 * 
 * */
#include <stdio.h>
#include <stdlib.h>
#include "shared.h"

void show_help() {
	printf("\n                          --  COMANDOS PRINCIPALES  --\n\n");
    printf("* mkdir (nombre del directorio) : Crea un directorio.\n");
    printf("* rmdir (nombre del directorio) : Elimina un directorio.\n");
    printf("* create (nombre del archivo) : Crea un archivo.\n");
    printf("* show (nombre del archivo) : Muestra el contenido del archivo.\n");
    printf("* dirlist (ruta) : Muestra un listado con el contenido del directorio.\n");
    printf("* chperm (nombre del archivo) (permiso_nuevo) : Cambia los permisos del archivo solicitado a los nuevos parámetros ingresados.\n");
    printf("* quit : Termina la ejecución de la minishell.\n\n");
	print_path();
}

int main() {
	show_help();
	return 0;
}
