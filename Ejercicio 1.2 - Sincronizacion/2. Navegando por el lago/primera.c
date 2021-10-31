#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>

struct msg{
	long tipo;
	sem_t salida;
	sem_t sem_turista;
	sem_t sem_business;
	sem_t sem_primera;
	sem_t entrada;
	sem_t ventaTicket;
}mensaje;

int main(){
	int longitud=sizeof(struct msg)-sizeof(long);
	key_t key;
	key=ftok("Sincronizacion2procesos",10);
	int idmsg=msgget(key, 0666);
	
	//----Seccion critica entrada al barco
	msgrcv(idmsg,&mensaje,longitud,1,0);
	sem_wait(&mensaje.entrada);
	fflush(stdout);
	sleep(1);
	printf("Pasajero clase Primera entro al barco\n");
	sem_post(&mensaje.ventaTicket);
	msgsnd(idmsg,&mensaje,longitud,0);
	//------------------------------------
	
	//----Seccion critica salida del barco
	sem_wait(&mensaje.salida);
	printf("Pasajero clase Primera salio del barco\n");
	fflush(stdout);
	sleep(1);
	sem_post(&mensaje.sem_primera);
	sem_post(&mensaje.salida);
	pthread_exit(0);
	//------------------------------------	
	
	
	return 0;
}
