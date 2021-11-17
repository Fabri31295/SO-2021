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

#define NUMERO 1
#define ESCRITOR1 2
#define ESCRITOR2 3

void generador(int id);
void sincronizador(int msgid);
void escritor(int id);
void control();
int random_number();

typedef struct message {
	int type; 
	int number;
	int writer;
} tMessage;

#define KEY 34251 
#define SIZE_MSG sizeof(tMessage)

int random_number() {	
	fflush(stdout);	
	sleep(1);
	return rand() % 1000;
}

void generador(int id) { 
	tMessage msg; 
	int msgid = msgget(KEY, 0666); 

	while(1){
		msg.number = random_number(); 
		msg.type = NUMERO; 
		printf("G%d generando: %d...\n",id,msg.number);
		msgsnd(msgid, &msg, SIZE_MSG, 0); 
	}
}

void sincronizador(int msgid) { 
	tMessage msg_recibido, msg_escritor;
	int destino = 1; 

	while(1){
		msgrcv(msgid, &msg_recibido, SIZE_MSG, NUMERO, 0);
		
		if(msg_recibido.number == -1) {
			printf("\n*** Cambio al escritor: %d\n\n",msg_recibido.writer);
			destino = msg_recibido.writer;
		} 
		else {
	
		switch(destino){
			case 1: msg_escritor.type = ESCRITOR1; //ahora escribe el 1
					break;
			case 2: msg_escritor.type = ESCRITOR2; //ahora escribe el 2
					break;
			default: printf("No hay donde enviar el msg -ERROR sinc- %n\n", &destino);
					
		}
		
		msg_escritor.number = msg_recibido.number;
		msgsnd(msgid, &msg_escritor, SIZE_MSG, 0);
		sleep(1);
		}
	}
}

void escritor(int id) { 
	tMessage msg; 
	FILE *file;
	int msgid = msgget(KEY, 0666); 
	
	while(1) { 
		switch(id){
			case 1: 
				msgrcv(msgid, &msg, SIZE_MSG, ESCRITOR1, 0);
				file = fopen("Salida1.txt","a+");
				printf("Escritor%d: %d\n", id, msg.number);
				fprintf(file,"%d\n", msg.number);
				fclose(file);
				break;
			case 2: 
				msgrcv(msgid, &msg, SIZE_MSG, ESCRITOR2, 0);
				file = fopen("Salida2.txt","a+");
				printf("Escritor%d: %d\n", id, msg.number);
				fprintf(file,"%d\n", msg.number);
				fclose(file);
				break;
			default :
				printf("Error en escritor\n");
		}
	}
}

void controlador() {
	tMessage msg;
	int destino = 1; 
	int msgid = msgget(KEY, 0666); 

	while(1){
		fflush(stdout);
		sleep(2);
		if(destino == 1)
			destino = 2;
		else 
			destino = 1;
		msg.type = NUMERO;
		msg.number = -1;
		msg.writer = destino; 
		msgsnd(msgid, &msg, SIZE_MSG, 0);
	}
}


int main() {
    int pid;
	int msgid = msgget(KEY, IPC_CREAT | 0666); //crea la cola
	if(msgid < 0) {
		printf("Error al crear la cola de mensajes \n");
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
		else if(fork() == 0){
			escritor(1);
			exit(EXIT_SUCCESS);
			}
			else if(fork() == 0){
					escritor(2);
					exit(EXIT_SUCCESS);
				}else if(fork() == 0){
							controlador();
							exit(EXIT_SUCCESS);
						} 
	
	if(pid > 0){ 
		sincronizador(msgid);
	}

	wait(NULL);
	wait(NULL);
	wait(NULL);
	
	msgctl(msgid, IPC_RMID, NULL);  
	exit(EXIT_SUCCESS);

    return 0;
}
