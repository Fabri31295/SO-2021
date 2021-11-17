#include "shared.h"

struct pasajeros{
	long tipo;
}spasajeros;

int main(){
	int longitud=sizeof(struct pasajeros)-sizeof(long);
	int idmsg2 = msgget(KEY2, 0666);
	
	while(1){
		
		//----Entrada al barco
		if(msgrcv(idmsg2,&spasajeros,longitud,PUEDE_ENTRAR_PRIMERA,IPC_NOWAIT)!=-1){
			msgrcv(idmsg2,&spasajeros,longitud,PUEDE_ENTRAR_BARCO,0);
			printf("Se vendio un ticket de clase Primera\n");
			fflush(stdout);
			sleep(1);
			printf("Pasajero clase Primera entro al barco\n");
			msgrcv(idmsg2,&spasajeros,longitud,BARCO_NO_LLENO,0);
			spasajeros.tipo=PUEDE_ENTRAR_BARCO;
			msgsnd(idmsg2,&spasajeros,longitud,0);
			//------------------------------------
			
			//----Salida del barco
			msgrcv(idmsg2,&spasajeros,longitud,PUEDE_SALIR_BARCO,0);
			msgrcv(idmsg2,&spasajeros,longitud,BARCO_NO_VACIO,0);
			printf("Pasajero clase Primera salio del barco\n");
			fflush(stdout);
			sleep(1);
			spasajeros.tipo=PUEDE_SALIR_BARCO;
			msgsnd(idmsg2,&spasajeros,longitud,0);
			//------------------------------------	
		}
	}
	
	return 0;
}
