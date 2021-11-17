#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <errno.h>

#define KEY 34251

int main(){
	int msgid = msgget(KEY, 0666);
	int a = msgctl(msgid, IPC_RMID, NULL);
	if(a==-1)
		printf("No existe la cola\n");
	else
		printf("Se eliminio la cola\n");
	return 0;
}
