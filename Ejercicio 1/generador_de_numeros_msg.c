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

#define CONTROL 4
#define GENERADOR 1
#define ESCRITOR1 2 
#define ESCRITOR2 3

void generador(int id);
void sincronizador(int msgid);
void escritor(int id);
void control();
int random_number();

typedef struct message {
	long type;
	int number;
} tMessage;

#define KEY 34251
#define SIZE_MSG sizeof(tMessage)

int random_number() {		
	int number; 
	number = rand() % 1000;
	return number;
}

void generador(int id) {
	tMessage message;
	int msgid = msgget(KEY, 0666);

	for(int i = 0; i < 4; i++) {
		message.number = random_number();
		message.type = 1;
		printf("G%d: generando: %d ...\n",id,message.number);
		msgsnd(msgid,&message,SIZE_MSG,0);
		fflush(stdout);
		sleep(1);
	}
}

void sincronizador(int msgid) {
	tMessage msg_generador, msg_control, msg_escritor;
	int destino = 1;

	for(int i = 0; i < 8; i++) {
		msgrcv(msgid, &msg_generador, SIZE_MSG, 1, 0);
		if(msgrcv(msgid, &msg_control, SIZE_MSG, 4, IPC_NOWAIT) == -1) {
    		
    		if (errno == ENOMSG) {
        		/* error */
    		}
    		else {
        		printf("Error al recibir el mensaje: %s\n", strerror(errno));
    		}
		}
		else {
    		printf("Cambio en la variable de control -->  %d\n",msg_control.number);
			destino = msg_control.number;
		}

		if(destino == 1) {
			msg_escritor.type = ESCRITOR1;
		} else if (destino == 2){
			msg_escritor.type = ESCRITOR2;
		}
		msg_escritor.number = msg_generador.number;
		msgsnd(msgid, &msg_escritor, SIZE_MSG, 0);
		sleep(1);
	}
}

void escritor(int id) { 
	tMessage msg_sync;
	int msgid = msgget(KEY, 0666);
	FILE *file;

	while(1) {
		if(id == ESCRITOR1) {
			msgrcv(msgid, &msg_sync, SIZE_MSG, ESCRITOR1, 0);
			file = fopen("Salida1.txt","a+");
			printf("Escritor 1 recibe: %d\n",msg_sync.number);
			fprintf(file,"%d\n",msg_sync.number);
			fclose(file);
		} else if(id == ESCRITOR2) {
			msgrcv(msgid, &msg_sync, SIZE_MSG, ESCRITOR2, 0);
			file = fopen("Salida2.txt","a+");
			printf("Escritor 2 recibe: %d\n",msg_sync.number);	
			fprintf(file,"%d\n",msg_sync.number);
			fclose(file);
		}
	}
}

void control() {
	tMessage msg_control;
	int msgid = msgget(KEY, 0666);
	int destino = 1;

	for(int i = 0; i < 3; i++) {
		fflush(stdout);
		sleep(2);
		if(destino == 1)
			 destino = 2;
		else 
			destino = 1;

		msg_control.type = 4;
		msg_control.number = destino;
		msgsnd(msgid, &msg_control, SIZE_MSG, 0);
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