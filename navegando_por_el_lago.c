#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

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
	sem_post(&salida);
	pthread_exit(0);
	//------------------------------------		
}


int main(){
	sem_init(&entrada,0,0);
	sem_init(&ventaTicket,0,0);
	sem_init(&sem_turista,0,50);
	sem_init(&sem_business,0,30);
	sem_init(&sem_primera,0,20);
	sem_init(&salida,0,0);
	
	srand(time(NULL));
	pthread_t pasajero[100];
	int pasajerosEnBarco=0;
	
	printf("Se comienza a vender los tickets y el barco vacio comienza a llenarse...\n");
	while(pasajerosEnBarco<100){
		int tipoPasajero=rand()%3; //0 turista, 1 business, 2 primera clase
		if (tipoPasajero==0){
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
		if (tipoPasajero==1){
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
		if (tipoPasajero==2){
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
	
	
	if(pasajerosEnBarco==100){
		printf("Barco lleno, parte del puerto...\n");
		fflush(stdout);
		sleep(5);
		
	}
	
	//se comienza a vaciar el barco
	printf("Barco llego a destino, comienza a vaciarse...\n");
	sem_post(&salida);
	
	for(int i=0; i<100 ;i++){
		pthread_join(pasajero[i],NULL);
	}
	
	printf("Barco vacio.\n");

	return 0;
}


