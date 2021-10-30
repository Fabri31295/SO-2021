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

#define RMDIR 0 
#define MKDIR 1
#define CREATE 2
#define DIRLIST 3
#define SHOW 4
#define CHPERM 5
#define HELP 6
#define QUIT 7
#define ARG_INVALIDOS -1

#define PRINT_ROJO_B "\x1b[1;31m"
#define PRINT_VERDE_B "\x1b[1;32m"
#define PRINT_AMARILLO_B "\x1b[1;33m"
#define PRINT_AZUL_B "\x1b[1;34m"
#define PRINT_MAGENTA_B "\x1b[1;35m"
#define PRINT_CYAN_B "\x1b[1;36m"
#define PRINT_RESET "\x1b[0m"

void report_and_exit(const char* msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

void print_path() {
	char ruta[PATH_MAX];
	getcwd(ruta,sizeof(ruta)); // almaceno la direccion actual en la que se encuentra el user
	printf(PRINT_VERDE_B"%s"PRINT_RESET,getenv("USER")); // imprimo el user actual por pantalla
	fflush(stdout);
	printf(PRINT_AZUL_B"%s"PRINT_RESET"$ ",ruta); // imprimo la direccion actual por pantalla
	fflush(stdout);
}
