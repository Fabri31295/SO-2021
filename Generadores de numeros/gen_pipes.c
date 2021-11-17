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
#include <signal.h>
#include <sys/wait.h>

void generador(int id);
void sincronizador();
void escritor1();
void escritor2();
int random_number();

typedef struct message {
	int number;
	int escritor;
} tMessage;

#define FINISH exit
#define SIZE_MSG sizeof(tMessage)
#define ESCRITOR1 1
#define ESCRITOR2 2

int p_number[2];
int p_write1[2];
int p_write2[2];

void generador(int id) {
    tMessage response;

    close(p_number[0]);
    close(p_write1[0]);
    close(p_write1[1]);
    close(p_write2[0]);
    close(p_write2[1]);

    for(int i = 0; i < 6; i++) {
        response.number = random_number();
        printf("Generador %d --> %d\n", id, response.number);
	write(p_number[1], &response, SIZE_MSG);
    }

    close(p_write1[1]);
}

int random_number() {
    int number; 
	srand(time(NULL)*getpid());
	number = rand() % 1000;
	fflush(stdout);
	sleep(1);
	return number;
}

void sincronizador() {
    tMessage request, response;
    int nread;
    int salida = 1;

    close(p_write1[0]);
    close(p_write2[0]);
    close(p_number[1]);

    for(int i = 0; i < 12; i++) {
        read(p_number[0], &request, SIZE_MSG);
	
	    if(request.number == -1)  // mensaje del controlador
	      salida = request.escritor;
    	else {
	      response.number = request.number;
	      if(salida == ESCRITOR1) 
	    	write(p_write1[1], &response, SIZE_MSG);
	     else 
		    write(p_write2[1], &response, SIZE_MSG);
	}
	
    }

}

void control() {
    tMessage msg_ctrl;
    int ubicacion = ESCRITOR1;
    
    close(p_number[0]);
    close(p_write1[0]);
    close(p_write2[0]);
    close(p_write1[1]);
    close(p_write2[1]);
    
    while(1) {
	if(ubicacion == ESCRITOR1)
	    ubicacion = ESCRITOR2;
	else 
	    ubicacion = ESCRITOR1;
	fflush(stdout);
	sleep(2);
	msg_ctrl.number = -1;
	msg_ctrl.escritor = ubicacion;
	write(p_number[1], &msg_ctrl, SIZE_MSG);
	printf("\nCambio en la variable de control: %d\n\n", msg_ctrl.escritor);	
    }
    
    close(p_number[0]);
}

void escritor1() {
    tMessage request;
    FILE *salida1;

    close(p_number[0]);
    close(p_number[1]);
    close(p_write1[1]);
    close(p_write2[0]);
    close(p_write2[1]);

    while(1) {
        read(p_write1[0], &request, SIZE_MSG);
        printf("Salida1: %d\n",request.number);
	salida1 = fopen("Salida1.txt","a+");
	fprintf(salida1, "%d\n",request.number);
	fclose(salida1);
    }

    close(p_write1[0]);

}

void escritor2() {
    tMessage request;
    FILE *salida2;

    close(p_number[0]);
    close(p_number[1]);
    close(p_write2[1]);
    close(p_write1[0]);
    close(p_write1[1]);

    while(1) {
        read(p_write2[0], &request, SIZE_MSG);
        printf("Salida2: %d\n",request.number);
	salida2 = fopen("Salida2.txt","a+");
	fprintf(salida2, "%d\n",request.number);
	fclose(salida2);
    }

    close(p_write2[0]);

}

int main() {
    int tmp, esc1, esc2, ctrl;
    pid_t pid;
    
    tmp = pipe(p_number);
    if(tmp == -1){
	perror("p_number");
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

    pid = fork();
    if(pid == 0) {
	generador(1);
	exit(EXIT_SUCCESS);
    } else if(fork() == 0) {
	generador(2);
	exit(EXIT_SUCCESS);
    } 
    esc1 = fork();
    if(esc1 == 0) {
	escritor1();
	exit(EXIT_SUCCESS);
    }
    esc2 = fork();
    if(esc2 == 0) {
	escritor2();
	exit(EXIT_SUCCESS);
    }
    ctrl = fork();
    if(ctrl == 0) {
	control();
	exit(EXIT_SUCCESS);
    }

    if(pid > 0) {
	sincronizador();
	kill(ctrl, SIGKILL);
	sleep(2);
        kill(esc1, SIGKILL);
	kill(esc2, SIGKILL);
    }

    printf("**** Fin ****\n");
    wait(NULL);
    wait(NULL);

    exit(EXIT_SUCCESS);

	return 0;
}
