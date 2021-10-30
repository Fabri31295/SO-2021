#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio_ext.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <sys/wait.h>
#include "shared.h"

#define COM_MAX_SIZE 30
#define MAX_ARG 4


/*retorna la cantidad de argumentos leidos en el comando pasado por parámetro.
    a su vez, almacena dichos argumentos en un arreglo de strings.
*/
int obtener_argumentos(char *comando, char *argumentos []){
    int cont = 0;
    argumentos[cont] = strtok(comando," ");   //almacena en el primer elemento el nombre del comando.
    while(argumentos[cont] != NULL && cont < MAX_ARG){    //mientras que haya elementos y los argumentos no superen la cantidad de 4.
        cont++;
        argumentos[cont] = strtok(NULL," ");    //almacena el siguiente argumento del comando.
    }
    return cont;
}

/*Función que analiza los argumentos escritos por el usuario y retorna -1 si no son válidos, en caso contrario
retorna el identificador del comando ingresado.*/
int argumentos_validos(char *argumentos [], int cant_argumentos){
    int comando = ARG_INVALIDOS;
    if(strcmp(argumentos[0],"rmdir") == 0 && (cant_argumentos > 1))
        comando = RMDIR;
    else
        if(strcmp(argumentos[0],"mkdir") == 0 && (cant_argumentos > 1))
            comando = MKDIR;
        else
            if(strcmp(argumentos[0],"create") == 0 && (cant_argumentos > 1))
                comando = CREATE;
            else
                if(strcmp(argumentos[0],"dirlist") == 0 && (cant_argumentos > 1))
                    comando = DIRLIST;
                else
                    if(strcmp(argumentos[0],"show") == 0 && (cant_argumentos > 1))
                        comando = SHOW;
                    else
                        if(strcmp(argumentos[0],"chperm") == 0 && (cant_argumentos == 3))
                            comando = CHPERM;
                        else
                            if(strcmp(argumentos[0],"help") == 0 && (cant_argumentos == 1))
                                comando = HELP;
                            else
                                if(strcmp(argumentos[0],"quit") == 0 && (cant_argumentos == 1))
                                    comando = QUIT;
    return comando;
}

int main(){
    printf(PRINT_AMARILLO_B"*****************************************************************************************\n");
    printf("**************************************--MINISHELL--**************************************\n");
    printf("******Escriba el comando 'help' para mostrar un listado con los comandos disponibles*****\n");
    printf("*****************************************************************************************\n"PRINT_RESET);
    print_path();    //imprime la ruta junto con el usuario actual
    char comando[COM_MAX_SIZE]; //aquí se almacena el comando escrito por teclado por el usuario.
    char *argumentos[MAX_ARG];    //sirve para almacenar los distintos argumentos que hay en el string comando.
    for(int i = 0; i < MAX_ARG; i++)
        argumentos[i] = (char *)malloc(sizeof(char)*20);
    fflush(stdin); //limpio el buffer de entrada del teclado.
    while(1){   //el programa no termina hasta que el usuario use el comando 'salir'
        for(int i = 0; i < MAX_ARG; i++)
            argumentos[i] = NULL;   //limpio el arreglo de argumentos antes de empezar una nueva iteración.
        int cant_argumentos = 0;
        scanf("%[^\n]s",comando);   //almacena en la variable comando todo lo que escriba el usuario hasta que presiona la tecla enter.
        __fpurge(stdin);    //limpio el buffer del teclado.
        pid_t pid;
        cant_argumentos = obtener_argumentos(comando,argumentos);   //obtengo la cantida de argumentos y los almaceno en la variable 'argumentos'.
        int comando_id = argumentos_validos(argumentos,cant_argumentos);
        if(comando_id == ARG_INVALIDOS){
            printf("El comando ingresado es inválido, escriba 'help' para ver la lista de comandos disponibles\n");
            print_path();
        }else{
            if((pid = fork()) < 0)  //creo el proceso que se ocupa de  ejecutar el comando recibido por teclado.
                report_and_exit("FORK");
            if(pid == 0){   //estoy en el proceso hijo.
                switch(comando_id){
                    case RMDIR:
                        execv("rmdir",argumentos);
                        break;
                    case MKDIR:
                        execv("mkdir",argumentos);
                        break;
                    case CREATE:
                        execv("create",argumentos);
                        break;
                    case DIRLIST:
                        execv("dirlist",argumentos);
                        break;
                    case SHOW:
                        execv("show",argumentos);
                        break;
                    case CHPERM:
                        execv("chperm",argumentos);
                        break;
                    case HELP:
                        execv("help",argumentos);
                        break;
                    case QUIT:
                        execv("quit",argumentos);
                        break;
                }
            }
        }
        
    }

    return 0;
}
