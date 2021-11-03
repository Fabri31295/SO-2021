#include "shared.h"

struct msg{
	long tipo;
}tipoPasajero,cola;

int main(){
	int longitud=sizeof(struct msg)-sizeof(long);
	key_t key,key2;
	key=ftok("/Sincronizacion2procesos",10);
	key2=ftok("/Sincronizacion2procesos",50);
	int idmsg = msgget(key, 0666);
	int idmsg2 = msgget(key2, 0666);
	
	//----Seccion critica entrada al barco
	msgrcv(idmsg,&tipoPasajero,longitud,TURISTA,0);
	msgrcv(idmsg2,&cola,longitud,PUEDE_COMPRAR,0);
	printf("Se vendio un ticket de clase Turista\n");
	fflush(stdout);
	sleep(1);
	printf("Pasajero clase Turista entro al barco\n");
	cola.tipo=ENTRO_AL_BARCO;
	msgsnd(idmsg2,&cola,longitud,0);
	//------------------------------------
	
	//----Seccion critica salida del barco
	msgrcv(idmsg,&cola,longitud,PUEDE_SALIR_BARCO,0);
	printf("Pasajero clase Turista salio del barco\n");
	fflush(stdout);
	sleep(1);
	cola.tipo=PUEDE_SALIR_BARCO;
	msgsnd(idmsg2,&cola,longitud,0);
	//------------------------------------	
	
	
	return 0;
}

