/* 
 * Sistemas operativos 2021 
 * Almaraz Fabricio, Buñes Juan.
 * 
 * */
#include "shared.h"

void show_file(char *name) {
	FILE *file = fopen(name,"r");
	char *line = NULL;
	size_t len = 0;
	size_t read;
	if(file == NULL) {
		perror("Error al abrir el archivo");
	}
	else {
		while((read = getline(&line, &len, file)) != -1) {
			printf("%s",line);
		}
		
		fclose(file);
		if(line)
			free(line);
	}
	print_path();
}

int main(int argc, int *argv[]) {
	show_file(argv[1]);
	return 0;
}
