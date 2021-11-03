#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>

#define TURISTA 0
#define BUSINESS 1
#define PRIMERA 2

#define PUEDE_COMPRAR 4
#define ENTRO_AL_BARCO 8
#define PUEDE_SALIR_BARCO 3

