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

#define TURISTA 0
#define BUSINESS 1
#define PRIMERA 2

#define MAX_TURISTA 50
#define MAX_BUSSINESS 30
#define MAX_PRIMERA 20
#define MAX_PASAJEROS 100

sem_t salida;
sem_t sem_turista;
sem_t sem_business;
sem_t sem_primera;
sem_t entrada;
sem_t ventaTicket;


//20 en primera, 30 en business, 50 en turista
//asumimos que se puede entrar y salir al barco de a un pasajero a la vez
//tambien se asume que se puede comprar de a un ticket a la vez

//Deberia hacer varios ciclos del barco??
//Problema semaforo ventaTicket, si no es incluido el ciclo while finaliza antes de que los hilos ejecuten

void *turista(){
	//----Seccion critica entrada al barco
	sem_wait(&entrada);
	fflush(stdout);
	sleep(1);
	printf("Pasajero clase Turista entro al barco\n");
	sem_post(&ventaTicket);
	//------------------------------------
		
	//----Seccion critica salida del barco
	sem_wait(&salida);
	printf("Pasajero clase Turista salio del barco\n");
	fflush(stdout);
	sleep(1);
	sem_post(&sem_turista);
	sem_post(&salida);
	pthread_exit(0);
	//------------------------------------
}


void *business(){
	//----Seccion critica entrada al barco
	sem_wait(&entrada);
	fflush(stdout);
	sleep(1);
	printf("Pasajero clase Business entro al barco\n");
	sem_post(&ventaTicket);
	//------------------------------------
	
	//----Seccion critica salida del barco
	sem_wait(&salida);
	printf("Pasajero clase Business salio del barco\n");
	fflush(stdout);
	sleep(1);
	sem_post(&sem_business);
	sem_post(&salida);
	pthread_exit(0);
	//------------------------------------	
}


void *primera(){
	//----Seccion critica entrada al barco
	sem_wait(&entrada);
	fflush(stdout);
	sleep(1);
	printf("Pasajero clase Primera entro al barco\n");
	sem_post(&ventaTicket);
	//------------------------------------
	
	//----Seccion critica salida del barco
	sem_wait(&salida);
	printf("Pasajero clase Primera salio del barco\n");
	fflush(stdout);
	sleep(1);
	sem_post(&sem_primera);
	sem_post(&salida);
	pthread_exit(0);
	//------------------------------------		
}


int main(){
	sem_init(&entrada,0,0);
	sem_init(&ventaTicket,0,0);
	sem_init(&sem_turista,0,MAX_TURISTA);
	sem_init(&sem_business,0,MAX_BUSSINESS);
	sem_init(&sem_primera,0,MAX_PRIMERA);
	sem_init(&salida,0,0);
	
	srand(time(NULL));
	pthread_t pasajero[MAX_PASAJEROS];
	while(1){
			
		int pasajerosEnBarco=0;
		printf("Se comienza a vender los tickets y el barco vacio comienza a llenarse...\n");
		
		while(pasajerosEnBarco<MAX_PASAJEROS){
			int tipoPasajero=rand()%3; //0 turista, 1 business, 2 primera clase
			if (tipoPasajero==TURISTA){
				if(sem_trywait(&sem_turista)==0){ //Hay lugar en el barco para turistas
					sem_post(&entrada);
					printf("Se vendio un ticket de clase Turista\n");
					pthread_create(&pasajero[pasajerosEnBarco],NULL,turista,NULL);
					pasajerosEnBarco=pasajerosEnBarco+1;
				}
				else{
					sem_post(&ventaTicket);
				}
			}
			if (tipoPasajero==BUSINESS){
				if(sem_trywait(&sem_business)==0){ //Hay lugar en el barco para business
					sem_post(&entrada);
					printf("Se vendio un ticket de clase Business\n");
					pthread_create(&pasajero[pasajerosEnBarco],NULL,business,NULL);
					pasajerosEnBarco=pasajerosEnBarco+1;
				}
				else{
					sem_post(&ventaTicket);
				}
			}
			if (tipoPasajero==PRIMERA){
				if(sem_trywait(&sem_primera)==0){ //Hay lugar en el barco para primera
					sem_post(&entrada);
					printf("Se vendio un ticket de clase Primera\n");
					pthread_create(&pasajero[pasajerosEnBarco],NULL,primera,NULL);
					pasajerosEnBarco=pasajerosEnBarco+1;
				}
				else{
					sem_post(&ventaTicket);
				}
			}
			sem_wait(&ventaTicket);	
		}
		
		
		if(pasajerosEnBarco==MAX_PASAJEROS){
			printf("Barco lleno, parte del puerto...\n");
			fflush(stdout);
			sleep(5);
			
		}
		
		//se comienza a vaciar el barco
		printf("Barco llego a destino, comienza a vaciarse...\n");
		sem_post(&salida);
		
		for(int i=0; i<MAX_PASAJEROS ;i++){
			pthread_join(pasajero[i],NULL);
		}
		
		sem_wait(&salida);
		printf("Barco vacio.\n");
		printf("-------------------------------------------\n");
	}
	return 0;
}


