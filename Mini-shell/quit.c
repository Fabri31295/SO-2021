/* 
 * Sistemas operativos 2021 
 * Almaraz Fabricio, Buñes Juan.
 * 
 * */
#include "shared.h"

void quit() {
	kill(getppid(), SIGTERM); // finaliza el proceso padre.
}

int main(int argc, char *argv[]) {
	quit();
	return 0;
}
