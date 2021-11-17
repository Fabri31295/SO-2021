#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_TURISTA 50
#define MAX_BUSINESS 30
#define MAX_PRIMERA 20
#define MAX_PASAJEROS 100

sem_t asientosVacios_turista;
sem_t asientosVacios_business;
sem_t asientosVacios_primera;
sem_t asientosVacios_barco;
sem_t asientosLlenos_barco;
sem_t entrada;
sem_t salida;


//20 en primera, 30 en business, 50 en turista
//Asumimos que se puede entrar y salir al barco de a un pasajero a la vez, es decir, son secciones criticas


void *turista(){
	while(1){
		if(sem_trywait(&asientosVacios_turista)==0){
			//----Seccion critica entrada al barco
			sem_wait(&entrada);
			fflush(stdout);
			sleep(1);
			sem_post(&asientosLlenos_barco);
			printf("Pasajero clase Turista compro ticket y entro al barco\n");
			sem_post(&entrada);
			//------------------------------------
				
			//----Seccion critica salida del barco
			sem_wait(&salida);
			printf("Pasajero clase Turista salio del barco\n");
			fflush(stdout);
			sleep(1);
			sem_post(&asientosVacios_turista);
			sem_post(&asientosVacios_barco);
			sem_post(&salida);	
			//------------------------------------
		}
	}
}


void *business(){
	while(1){
		if(sem_trywait(&asientosVacios_business)==0){
			//----Seccion critica entrada al barco
			sem_wait(&entrada);
			fflush(stdout);
			sleep(1);
			sem_post(&asientosLlenos_barco);
			printf("Pasajero clase Business compro ticket y entro al barco\n");
			sem_post(&entrada);
			//------------------------------------
				
			//----Seccion critica salida del barco
			sem_wait(&salida);
			printf("Pasajero clase Business salio del barco\n");
			fflush(stdout);
			sleep(1);
			sem_post(&asientosVacios_business);
			sem_post(&asientosVacios_barco);
			sem_post(&salida);
			//------------------------------------
		}
	}
}


void *primera(){
	while(1){
		if(sem_trywait(&asientosVacios_primera)==0){
			//----Seccion critica entrada al barco
			sem_wait(&entrada);
			fflush(stdout);
			sleep(1);
			sem_post(&asientosLlenos_barco);
			printf("Pasajero clase Primera compro ticket y entro al barco\n");
			sem_post(&entrada);
			//------------------------------------
				
			//----Seccion critica salida del barco
			sem_wait(&salida);
			printf("Pasajero clase Primera salio del barco\n");
			fflush(stdout);
			sleep(1);
			sem_post(&asientosVacios_primera);
			sem_post(&asientosVacios_barco);
			sem_post(&salida);	
			//------------------------------------
		}
	}	
}


int main(){
	sem_init(&asientosVacios_turista,0,MAX_TURISTA);
	sem_init(&asientosVacios_business,0,MAX_BUSINESS);
	sem_init(&asientosVacios_primera,0,MAX_PRIMERA);
	sem_init(&asientosVacios_barco,0,MAX_PASAJEROS);
	sem_init(&asientosLlenos_barco,0,0);
	sem_init(&entrada,0,0);
	sem_init(&salida,0,0);
	int error;
	pthread_t clientesTurista[100];
	pthread_t clientesBusiness[100];
	pthread_t clientesPrimera[100];
	for(int i=0; i<100; i++){
		error=pthread_create(&clientesTurista[i],NULL,turista,NULL);
		if(error!=0){
			return(1);
		}
		error=pthread_create(&clientesBusiness[i],NULL,business,NULL);
		if(error!=0){
			return(1);
		}
		error=pthread_create(&clientesPrimera[i],NULL,primera,NULL);
		if(error!=0){
			return(1);
		}
	}
		
	
	while(1){
		sem_post(&entrada);//Se habilita la venta de tickets y la entrada al barco
		
		printf("Se comienza a vender los tickets y el barco vacio comienza a llenarse...\n");
		
		//Cicla 100 veces esperando 100 veces al semaforo asientosLlenos_Barco
		for(int i=0; i<100;i++){
			sem_wait(&asientosLlenos_barco);	
		}
		sem_wait(&entrada);//Se deshabilita la venta de tickets y entrada al barco
		printf("Barco lleno, parte del puerto...\n");
		fflush(stdout);
		sleep(5);
		
		printf("Barco llego a destino, comienza a vaciarse...\n");
		sem_post(&salida);//Se habilita la salida del barco
		
		//Cicla 100 veces esperando 100 veces al semaforo asientosVacios_Barco
		for(int i=0; i<100;i++){
			sem_wait(&asientosVacios_barco);	
		}
		
		sem_wait(&salida);//Se deshabilita la salida del barco
		printf("Barco vacio.\n");
		printf("-------------------------------------------\n");
	}
	
	return 0;
}


