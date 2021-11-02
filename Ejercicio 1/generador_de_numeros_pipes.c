/* 
 * Sistemas operativos 2021 
 * Almaraz Fabricio, Buñes Juan.
 * 
 * */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>

void generador();
void sincronizador();
void escritor1();
void escritor2();
int random_number();

typedef struct message {
	int c;
} tMessage;

#define FINISH exit
#define SIZE_MSG sizeof(tMessage)

int p_signal[2];
int p_number[2];
int p_write1[2];
int p_write2[2];
int p_ctrl[2];

void generador(int id) {
	tMessage request,response;
	
	close(p_number[0]); // cierro para lectura
	close(p_write1[0]);
	close(p_write2[0]);
	close(p_ctrl[0]);
	close(p_signal[1]); // cierro para escritura
	close(p_write1[1]);
	close(p_write2[1]);
	close(p_ctrl[1]);
	
	char str[4];
	int number;
	while(1) {
		read(p_signal[0],&request,SIZE_MSG);
		number = random_number();
		response.c = number;
		printf("Generador %d --> %d\n", id, response.c);
		write(p_number[1], &response, SIZE_MSG);
	}
	
	close(p_signal[0]);
	close(p_number[1]);
	exit(EXIT_SUCCESS);
}

int random_number() {		
	int number; 
	srand(time(NULL));
	number = rand() % 1000;
	fflush(stdout);
	sleep(1);
	return number;
}

void sincronizador() {
	tMessage request, request_ctrl, response;
	int nread;
	int salida = 1;

	close(p_write1[0]); // cierro para lectura
	close(p_write2[0]);
	close(p_signal[0]);
	close(p_number[1]); // cierro para escritura
	close(p_signal[1]);
	close(p_ctrl[1]);

	while (1) {
		nread = read(p_ctrl[0], &request_ctrl, SIZE_MSG);
		switch (nread) {
		case -1:
			if (errno == EAGAIN) {
				read(p_number[0], &request, SIZE_MSG);
				response.c = request.c;
				if(salida == 1)
					write(p_write1[1], &response, SIZE_MSG);
				else
					write(p_write2[1], &response, SIZE_MSG);
				break;
			}
			else {
				perror("read");
				exit(4);
			}
		case 0:
			printf("End of conversation\n");
			close(p_ctrl[0]);

			exit(0);
		default:
			salida = request_ctrl.c;
		}
	}
	
	close(p_number[0]);
	close(p_write1[1]);
	close(p_write2[1]);
	exit(EXIT_SUCCESS);
}

void control() {
	tMessage response;
	int ubicacion = 1;
	close(p_ctrl[0]);

	while(1) {
		response.c = (++ubicacion)%2;
		if (response.c == 0)
			response.c = 2;
		fflush(stdout);
		sleep(rand()%7+3);
		write(p_ctrl[1], &response, SIZE_MSG);
		sleep(1);
		printf("\nCambio en la variable de control: %d\n\n", response.c);
	}

	exit(EXIT_SUCCESS);
}

void escritor1() {
	tMessage request, response;
	FILE *salida1;

	close(p_signal[0]); // cierro para lectura
	close(p_number[0]);
	close(p_write2[0]);
	close(p_ctrl[0]);
	close(p_write1[1]); // cierro para escritura
	close(p_write2[1]);
	close(p_number[1]);
	close(p_ctrl[1]);
	
	while(1) {
		read(p_write1[0], &request, SIZE_MSG);

		printf("Salida1: %d\n",request.c);
		salida1 = fopen("Salida1.txt","a+");
		fprintf(salida1, "%d\n",request.c);
		fclose(salida1);

		write(p_signal[1], &response, SIZE_MSG);
	}
	
	close(p_write1[0]);
	close(p_signal[1]);
	exit(EXIT_SUCCESS);
}

void escritor2() {
	tMessage request, response;
	FILE *salida2;
	
	close(p_signal[0]); // cierro para lectura
	close(p_number[0]);
	close(p_write1[0]);
	close(p_ctrl[0]);
	close(p_write1[1]); // cierro para escritura
	close(p_write2[1]);
	close(p_number[1]);
	close(p_ctrl[1]);
	
	while(1) {
		read(p_write2[0], &request, SIZE_MSG);

		printf("Salida2: %d\n",request.c);
		salida2 = fopen("Salida2.txt","a+");
		fprintf(salida2, "%d\n",request.c);
		fclose(salida2);

		write(p_signal[1], &response, SIZE_MSG);
	}
	
	close(p_write2[0]);
	close(p_signal[1]);
	exit(EXIT_SUCCESS);
}

int main() {
	int tmp;
	pid_t pid;
	tMessage response;
	
	tmp = pipe(p_number);
	if(tmp == -1){
		perror("p_number");
		exit(EXIT_FAILURE);
	}
	tmp = pipe(p_signal);
	if(tmp == -1){
		perror("p_signal");
		exit(EXIT_FAILURE);
	}
	tmp = pipe(p_write1);
	if(tmp == -1){
		perror("p_write1");
		exit(EXIT_FAILURE);
	}
	tmp = pipe(p_write2);
	if(tmp == -1){
		perror("p_write2");
		exit(EXIT_FAILURE);
	}
	tmp = pipe(p_ctrl);
	if(tmp == -1){
		perror("p_ctrl");
		exit(EXIT_FAILURE);
	}	

	if (fcntl(p_ctrl[0], F_SETFL, O_NONBLOCK) < 0)
		exit(2);
		
	pid = fork();
	
	if(pid == 0) {
		generador(1);
		exit(EXIT_SUCCESS);
	} else if(fork() == 0) {
		generador(2);
		exit(EXIT_SUCCESS);
	} else if(fork() == 0) {
		escritor1();
		exit(EXIT_SUCCESS);
	} else if(fork() == 0) {
		escritor2();
		exit(EXIT_SUCCESS);
	} else if(fork() == 0) {
		control();
		exit(EXIT_SUCCESS);
	}
	
	if(pid > 0) {
		write(p_signal[1], &response, SIZE_MSG);
		sincronizador();
	}

	return 0;
}
