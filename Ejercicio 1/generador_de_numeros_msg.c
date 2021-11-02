/* 
 * Sistemas operativos 2021 
 * Almaraz Fabricio, Buñes Juan.
 * 
 * */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <errno.h>

void generador(int id);
void sincronizador(int msgid);
void escritor(int id);
void control();
void delete_queue();
int random_number();

typedef struct message {
	long type;
	int number;
	int ctrl;
} tMessage;

#define KEY 34251
#define SIZE_MSG sizeof(tMessage)

int random_number() {		
	int number; 
	number = rand() % 1000;
	return number;
}

void generador(int id) {
	tMessage response;
	int msgid = msgget(KEY, 0666);

	for(int i = 0; i < 4; i++) {
		response.number = random_number();
		response.type = 1;
		printf("G%d: generando: %d ...\n",id,response.number);
		msgsnd(msgid,&response,SIZE_MSG,0);
		fflush(stdout);
		sleep(1);
	}
}

void sincronizador(int msgid) {
	tMessage request, control, response;
	int var_control = 1;

	for(int i = 0; i < 8; i++) {
		msgrcv(msgid, &request, SIZE_MSG, 1, 0);
		if(msgrcv(msgid, &control, SIZE_MSG, 4, IPC_NOWAIT) == -1) {
    		/* Possible error */
    		if (errno == ENOMSG) {
        		//printf("No message in the queue\n");
    		}
    		else {
        		printf("Error receiving message: %s\n", strerror(errno));
    		}
		}
		else {
    		printf("Cambio en la variable de control -->  %d\n",control.number);
			var_control = control.number;
		}

		response.ctrl = var_control;

		if(var_control == 1) {
			response.type = 2;
		} else if (var_control == 2){
			response.type = 3;
		}
		response.number = request.number;
		//printf("Envio %d para el escritor %d\n",response.number,var_control);
		msgsnd(msgid, &response, SIZE_MSG, 0);
		sleep(1);
	}
}

void escritor(int id) { 
	tMessage request;
	int msgid = msgget(KEY, 0666);
	FILE *file;

	while(1) {
		if(id == 1) {
			msgrcv(msgid, &request, SIZE_MSG, 2, 0);
			file = fopen("Salida1.txt","a+");
			printf("Escritor 1 recibe: %d\n",request.number);
			fprintf(file,"%d\n",request.number);
			fclose(file);
		} else if(id == 2) {
			msgrcv(msgid, &request, SIZE_MSG, 3, 0);
			file = fopen("Salida2.txt","a+");
			printf("Escritor 2 recibe: %d\n",request.number);	
			fprintf(file,"%d\n",request.number);
			fclose(file);
		}
	}
}

void control() {
	tMessage control;
	int msgid = msgget(KEY, 0666);
	int variable = 1;

	for(int i = 0; i < 3; i++) {
		fflush(stdout);
		sleep(2);
		if(variable == 1) variable = 2;
		else variable = 1;
		control.type = 4;
		control.number = variable;
		msgsnd(msgid, &control, SIZE_MSG, 0);
	}
}

int main() {
    int pid, esc1, esc2;
	int msgid = msgget(KEY, IPC_CREAT | 0666);
	if(msgid < 0) {
		printf("Error al crear la cola de mensajes\n");
		exit(1);
    }	

    pid = fork();
	if(pid == 0) {
		srand(getpid());
		generador(1);
		exit(EXIT_SUCCESS);
	}
	else if(fork() == 0){
		srand(getpid());
		generador(2);
		exit(EXIT_SUCCESS);
	}  
	esc1 = fork();
	if(esc1 == 0){
		escritor(1);
		exit(EXIT_SUCCESS);
	} 
	esc2 = fork();
	if(esc2 == 0){
		escritor(2);
		exit(EXIT_SUCCESS);
	} 
	else if(fork() == 0){
		control();
		exit(EXIT_SUCCESS);
	} 

	if(pid > 0){
		sincronizador(msgid);
		kill(esc1,SIGKILL);
		kill(esc2,SIGKILL);
	}

	wait(NULL);
	wait(NULL);
	wait(NULL);

	msgctl(msgid, IPC_RMID, NULL); // elimino la cola de mensajes
	exit(EXIT_SUCCESS);

    return 0;
}