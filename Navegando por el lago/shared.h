#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>

#define TURISTA 111
#define BUSINESS 222
#define PRIMERA 333

#define KEY2 33227

#define MAX_TURISTA 50
#define MAX_BUSINESS 30
#define MAX_PRIMERA 20
#define MAX_PASAJEROS 100

#define PUEDE_ENTRAR_BARCO 40
#define ENTRO_AL_BARCO 80
#define PUEDE_SALIR_BARCO 60
#define SALIO_DEL_BARCO 150
#define PUEDE_ENTRAR_TURISTA 190
#define PUEDE_ENTRAR_BUSINESS 180
#define PUEDE_ENTRAR_PRIMERA 170
#define BARCO_NO_VACIO 260
#define BARCO_NO_LLENO 270


