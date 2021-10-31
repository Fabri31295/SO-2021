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
	printf("proceso business\n");
	int longitud=sizeof(struct msg)-sizeof(long);
	key_t key;
	key=ftok("Sincronizacion2procesos",10);
	int idmsg=msgget(key, 0666);
	
	msgrcv(idmsg,&mensaje,longitud,1,0);
	
	return 0;
}
