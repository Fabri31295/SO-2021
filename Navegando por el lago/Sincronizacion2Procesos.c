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
#include <sys/types.h>
#include <sys/ipc.h>

#define TURISTA 0
#define BUSINESS 1
#define PRIMERA 2

#define PUEDE_COMPRAR 4
#define ENTRO_AL_BARCO 8
#define PUEDE_SALIR_BARCO 3
#define MAX_TURISTA 50
#define MAX_BUSSINESS 30
#define MAX_PRIMERA 20
#define MAX_PASAJEROS 100


struct msg{
	long tipo;
	
}tipoPasajero,cola;

int main(){
	int longitud=sizeof(struct msg)-sizeof(long);
	key_t key,key2;
	key=ftok("Sincronizacion2procesos",10);
	key2=ftok("Sincronizacion2procesos",50);
	int idmsg=msgget(key, 0666 | IPC_CREAT);
	int idmsg2=msgget(key2, 0666 | IPC_CREAT);
	
	srand(time(NULL));
	
	while(1){
		printf("Se comienza a vender los tickets y el barco vacio comienza a llenarse...\n");	
		int pasajerosEnBarco=0;
		int cantTurista=0;
		int cantBusiness=0;
		int cantPrimera=0;
		
		while(pasajerosEnBarco<MAX_PASAJEROS){		
			tipoPasajero.tipo=rand()%3; //0 turista, 1 business, 2 primera
				if(tipoPasajero.tipo==PRIMERA){
					cola.tipo=PUEDE_COMPRAR;
					msgsnd(idmsg,&tipoPasajero,longitud,0);
					msgsnd(idmsg2,&cola,longitud,0);
					if(cantPrimera<MAX_PRIMERA){
						if(fork()==0){
							char *args[]={"./pri",NULL};
							execv(args[0],args);
						}
						pasajerosEnBarco=pasajerosEnBarco+1;
						cantPrimera++;
						msgrcv(idmsg2,&cola,longitud,ENTRO_AL_BARCO,0);
					}
					
				}
				if(tipoPasajero.tipo==BUSINESS){
					cola.tipo=PUEDE_COMPRAR;
					msgsnd(idmsg,&tipoPasajero,longitud,0);
					msgsnd(idmsg2,&cola,longitud,0);
					if(cantBusiness<MAX_BUSSINESS){
						if(fork()==0){
							char *args[]={"./bus",NULL};
							execv(args[0],args);
						}
						pasajerosEnBarco=pasajerosEnBarco+1;
						cantBusiness++;
						msgrcv(idmsg2,&cola,longitud,ENTRO_AL_BARCO,0);
					}
					
				}
				if(tipoPasajero.tipo==TURISTA){
					cola.tipo=PUEDE_COMPRAR;
					msgsnd(idmsg,&tipoPasajero,longitud,0);
					msgsnd(idmsg2,&cola,longitud,0);
					if(cantTurista<MAX_TURISTA){
						if(fork()==0){
							char *args[]={"./tur",NULL};
							execv(args[0],args);
						}
						pasajerosEnBarco=pasajerosEnBarco+1;
						cantTurista++;
						msgrcv(idmsg2,&cola,longitud,ENTRO_AL_BARCO,0);
					}
					
				}
			
		}
		
		if(pasajerosEnBarco==MAX_PASAJEROS){
				printf("Barco lleno, parte del puerto...\n");
				fflush(stdout);
				sleep(5);	
		}
			
		//se comienza a vaciar el barco
		printf("Barco llego a destino, comienza a vaciarse...\n");
			
		for(int i=0; i<MAX_PASAJEROS ;i++){
			cola.tipo=PUEDE_SALIR_BARCO;
			msgsnd(idmsg2,&cola,longitud,0);
			msgrcv(idmsg2,&cola,longitud,PUEDE_SALIR_BARCO,0);
		}
			
		for(int i=0; i<MAX_PASAJEROS ;i++){
				wait(NULL);
		}
		printf("Barco vacio.\n");	
		printf("-------------------------------------------\n");	
		
	}
	return 0;
}
