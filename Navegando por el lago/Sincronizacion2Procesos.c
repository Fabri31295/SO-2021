#include "shared.h"

struct pasajeros{
	long tipo;
}spasajeros;

int main(){
	int longitud=sizeof(struct pasajeros)-sizeof(long);
	
	
	//Crea 60 procesos de cada tipo de pasajero posible para que compitan por los tickets disponibles
	for(int i=0; i<60; i++){
		if(fork()==0){
			char *args[]={"./turista",NULL};
			execv(args[0],args);
		}	
	}

	for(int i=0; i<60; i++){
		if(fork()==0){
			char *args[]={"./business",NULL};
			execv(args[0],args);
		}	
	}
	
	for(int i=0; i<60; i++){
		if(fork()==0){
			char *args[]={"./primera",NULL};
			execv(args[0],args);
		}	
	}
	
	
	while(1){
		int idmsg2=msgget(KEY2, 0666|IPC_CREAT);
		//msgctl(idmsg2,IPC_RMID,NULL);
		//idmsg2=msgget(KEY2, 0666|IPC_CREAT);
		
		printf("Se comienza a vender los tickets y el barco vacio comienza a llenarse...\n");	
		
		//Envia MAX_PRIMERA mensajes que permiten dejar entrar a esa cantidad de pasajeros de clase primera al barco
		for(int i=0; i<MAX_PRIMERA; i++){
			spasajeros.tipo=PUEDE_ENTRAR_PRIMERA;
			int er=msgsnd(idmsg2,&spasajeros,longitud,0);	
			if(er==-1){
				return -1;
			}
		}
		//Envia MAX_BUSINESS mensajes que permiten dejar entrar a esa cantidad de pasajeros de clase business al barco
		for(int i=0; i<MAX_BUSINESS; i++){
			spasajeros.tipo=PUEDE_ENTRAR_BUSINESS;
			int er=msgsnd(idmsg2,&spasajeros,longitud,0);	
			if(er==-1){
				return -1;
			}
		}
		//Envia MAX_TURISTA mensajes que permiten dejar entrar a esa cantidad de pasajeros de clase turista al barco
		for(int i=0; i<MAX_TURISTA; i++){
			spasajeros.tipo=PUEDE_ENTRAR_TURISTA;
			int er=msgsnd(idmsg2,&spasajeros,longitud,0);	
			if(er==-1){
				return -1;
			}
		}
		//Envia MAX_PASAJEROS mensajes que permiten dejar entrar a esa cantidad de pasajeros al barco
		for(int i=0; i<MAX_PASAJEROS; i++){
			spasajeros.tipo=BARCO_NO_LLENO;
			int er=msgsnd(idmsg2,&spasajeros,longitud,0);	
			if(er==-1){
				return -1;
			}
		}
		spasajeros.tipo=PUEDE_ENTRAR_BARCO; 
		int er1=msgsnd(idmsg2,&spasajeros,longitud,0); //Comienza a dejar entrar a los pasajeros
		if(er1==-1){
			return -1;
		}

		//Cicla hasta que el barco este lleno, cuando deje de recibir mensajes de que el barco no esta lleno, significa
		//que esta lleno, sale del ciclo while y puede zarpar
		while(msgrcv(idmsg2,&spasajeros,longitud,BARCO_NO_LLENO,IPC_NOWAIT)!=-1){
			spasajeros.tipo=BARCO_NO_LLENO;
			int er=msgsnd(idmsg2,&spasajeros,longitud,0);	
			if(er==-1){
				return -1;
			}
		}
		msgrcv(idmsg2,&spasajeros,longitud,PUEDE_ENTRAR_BARCO,0);//Consume el mensaje para que no pueda seguir entrando al barco
			
		printf("Barco lleno, parte del puerto...\n");
		fflush(stdout);
		sleep(5);	
		printf("Barco llego a destino, comienza a vaciarse...\n");
		
		
		for(int i=0; i<MAX_PASAJEROS; i++){
			spasajeros.tipo=BARCO_NO_VACIO;
			int er=msgsnd(idmsg2,&spasajeros,longitud,0);	
			if(er==-1){
				return -1;
			}
		}
			
		spasajeros.tipo=PUEDE_SALIR_BARCO; 
		int er2=msgsnd(idmsg2,&spasajeros,longitud,0); //Comienza a dejar salir a los pasajeros
		if(er2==-1){
			return -1;
		}	
		//Cicla hasta que el barco este vacio, cuando deje de recibir mensajes de que el barco no esta vacio, significa
		//que esta vacio, sale del ciclo while y esta listo para comenzar a llenarse para un nuevo viaje	
		while(msgrcv(idmsg2,&spasajeros,longitud,BARCO_NO_VACIO,IPC_NOWAIT)!=-1){
			spasajeros.tipo=BARCO_NO_VACIO;
			int er=msgsnd(idmsg2,&spasajeros,longitud,0);	
			if(er==-1){
				return -1;
			}
		}
		msgrcv(idmsg2,&spasajeros,longitud,PUEDE_SALIR_BARCO,0);//Consume el mensaje para que no pueda seguir saliendo al barco
		
	
		printf("Barco vacio.\n");	
		printf("-------------------------------------------\n");	
		fflush(stdout);
		sleep(2);
	}
	return 0;
}
