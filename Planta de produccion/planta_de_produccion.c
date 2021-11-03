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


sem_t sem_a;
sem_t sem_b;
sem_t sem_c;
sem_t sem_d;
sem_t sem_e;
sem_t sem_f;

pthread_t thread_a;
pthread_t thread_b;
pthread_t thread_c;
pthread_t thread_d;
pthread_t thread_e;
pthread_t thread_f;

pthread_attr_t attr;

void *printA(){
	while(1){
		sem_wait(&sem_a);
		fflush(stdout);
		sleep(2);
		printf("%s","A");
		sem_post(&sem_b);
	}
}

void *printB(){
	while(1){
		sem_wait(&sem_b);
		fflush(stdout);
		sleep(1);
		printf("%s","B");
		sem_post(&sem_c);
		sem_wait(&sem_b);
		fflush(stdout);
		sleep(1);
		printf("%s","B");
		sem_post(&sem_d);
	}
}

void *printC(){
	while(1){
		sem_wait(&sem_c);
		fflush(stdout);
		sleep(3);
		printf("%s","C");
		sem_post(&sem_e);
	}
}

void *printD(){
	while(1){
		sem_wait(&sem_d);
		fflush(stdout);
		sleep(7);
		printf("%s","D");
		sem_post(&sem_e);
	}
}

void *printE(){
	while(1){
		sem_wait(&sem_e);
		fflush(stdout);
		sleep(2); 
		printf("%s","E");
		sem_post(&sem_f);
	}
}

void *printF(){
	while(1){
		sem_wait(&sem_f);
		fflush(stdout);
		sleep(3);
		printf("%s","F");
		sem_post(&sem_a);
	}
}

int main(){
		
	sem_init(&sem_a,0,1);
	sem_init(&sem_b,0,0);
	sem_init(&sem_c,0,0);
	sem_init(&sem_d,0,0);
	sem_init(&sem_e,0,0);
	sem_init(&sem_f,0,0);
	pthread_attr_init(&attr);
	
	pthread_create(&thread_a,&attr,printA,NULL);
	pthread_create(&thread_b,&attr,printB,NULL);
	pthread_create(&thread_c,&attr,printC,NULL);
	pthread_create(&thread_d,&attr,printD,NULL);
	pthread_create(&thread_e,&attr,printE,NULL);
	pthread_create(&thread_f,&attr,printF,NULL);
	
	pthread_join(thread_a,NULL);
	pthread_join(thread_b,NULL);
	pthread_join(thread_c,NULL);
	pthread_join(thread_d,NULL);
	pthread_join(thread_e,NULL);
	pthread_join(thread_f,NULL);
	
	pthread_exit(0);
	return 0;
}

