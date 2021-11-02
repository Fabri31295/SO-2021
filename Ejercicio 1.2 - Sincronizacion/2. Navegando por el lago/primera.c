/* 
 * Sistemas operativos 2021 
 * Almaraz Fabricio, Buñes Juan.
 * 
 * */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>

#define TURISTA 0
#define BUSINESS 1
#define PRIMERA 2

#define PUEDE_COMPRAR 4
#define ENTRO_AL_BARCO 8
#define PUEDE_SALIR_BARCO 3

struct msg{
	long tipo;
}tipoPasajero,cola;

int main(){
	int longitud=sizeof(struct msg)-sizeof(long);
	key_t key,key2;
	key=ftok("Sincronizacion2procesos",10);
	int idmsg=msgget(key, 0666);
	key2=ftok("Sincronizacion2procesos",50);
	int idmsg2=msgget(key2, 0666);
	
	//----Seccion critica entrada al barco
	msgrcv(idmsg,&tipoPasajero,longitud,PRIMERA,0);
	msgrcv(idmsg2,&cola,longitud,PUEDE_COMPRAR,0);
	printf("Se vendio un ticket de clase Primera\n");
	fflush(stdout);
	sleep(1);
	printf("Pasajero clase Primera entro al barco\n");
	cola.tipo=ENTRO_AL_BARCO;
	msgsnd(idmsg2,&cola,longitud,0);
	//------------------------------------
	
	//----Seccion critica salida del barco
	msgrcv(idmsg,&cola,longitud,PUEDE_SALIR_BARCO,0);
	printf("Pasajero clase Primera salio del barco\n");
	fflush(stdout);
	sleep(1);
	cola.tipo=PUEDE_SALIR_BARCO;
	msgsnd(idmsg2,&cola,longitud,0);
	//------------------------------------	
	
	
	return 0;
}
