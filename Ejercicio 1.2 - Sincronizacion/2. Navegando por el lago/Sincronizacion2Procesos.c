#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>

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
	printf("inicio proceso padre\n");
	sem_init(&mensaje.entrada,0,0);
	sem_init(&mensaje.ventaTicket,0,0);
	sem_init(&mensaje.sem_turista,0,50);
	sem_init(&mensaje.sem_business,0,30);
	sem_init(&mensaje.sem_primera,0,20);
	sem_init(&mensaje.salida,0,0);
	int longitud=sizeof(struct msg)-sizeof(long);
	key_t key;
	key=ftok("Sincronizacion2procesos",10);
	int idmsg=msgget(key, 0666 | IPC_CREAT);
	mensaje.tipo=1;
	
	srand(time(NULL));
	int pasajerosEnBarco=0;
	
	while(pasajerosEnBarco<100){
		int tipoPasajero=rand()%3; //0 turista, 1 business, 2 primera clase
		tipoPasajero=2;
			if (tipoPasajero==0){
				if(sem_trywait(&mensaje.sem_turista)==0){ //Hay lugar en el barco para turistas
					sem_post(&mensaje.entrada);
					printf("Se vendio un ticket de clase Turista\n");
					//pthread_create(&pasajero[pasajerosEnBarco],NULL,turista,NULL);
					pasajerosEnBarco=pasajerosEnBarco+1;
				}
				else{
					sem_post(&mensaje.ventaTicket);
				}
			}
			if (tipoPasajero==1){
				if(sem_trywait(&mensaje.sem_business)==0){ //Hay lugar en el barco para business
					sem_post(&mensaje.entrada);
					printf("Se vendio un ticket de clase Business\n");
					//pthread_create(&pasajero[pasajerosEnBarco],NULL,business,NULL);
					pasajerosEnBarco=pasajerosEnBarco+1;
				}
				else{
					sem_post(&mensaje.ventaTicket);
				}
			}
			if (tipoPasajero==2){
				if(sem_trywait(&mensaje.sem_primera)==0){ //Hay lugar en el barco para primera
					sem_post(&mensaje.entrada);
					printf("Se vendio un ticket de clase Primera\n");
					msgsnd(idmsg,&mensaje,longitud,0);
					if(fork()==0){
						char *args[]={"./pri",NULL};
						execv(args[0],args);
					}
					pasajerosEnBarco=pasajerosEnBarco+1;
				}
				else{
					sem_post(&mensaje.ventaTicket);
				}
			}
			msgrcv(idmsg,&mensaje,longitud,1,0);
			sem_wait(&mensaje.ventaTicket);	
		}
	
	
	wait(NULL);
	printf("fin proceso padre\n");
	
	
	return 0;
}
