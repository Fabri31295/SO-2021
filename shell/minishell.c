/* 
 * Sistemas operativos 2021 
 * Almaraz Fabricio, Buñes Juan.
 * 
 * */
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


/*retorna la cantidad de arguments leidos en el command pasado por parámetro.
    a su vez, almacena dichos arguments en un arreglo de strings.
*/
int get_arguments(char *command, char *arguments []){
    int cont = 0;
    arguments[cont] = strtok(command," ");   //almacena en el primer elemento el nombre del command.
    while(arguments[cont] != NULL && cont < MAX_ARG){    //mientras que haya elementos y los arguments no superen la cantidad de 4.
        cont++;
        arguments[cont] = strtok(NULL," ");    //almacena el siguiente argumento del command.
    }
    return cont;
}

/*Función que analiza los arguments escritos por el usuario y retorna -1 si no son válidos, en caso contrario
retorna el identificador del command ingresado.*/
int valid_arguments(char *arguments [], int cant){
    int command = ARG_INVALIDOS;
    if(strcmp(arguments[0],"rmdir") == 0 && (cant > 1))
        command = RMDIR;
    else
        if(strcmp(arguments[0],"mkdir") == 0 && (cant > 1))
            command = MKDIR;
        else
            if(strcmp(arguments[0],"create") == 0 && (cant > 1))
                command = CREATE;
            else
                if(strcmp(arguments[0],"dirlist") == 0 && (cant > 1))
                    command = DIRLIST;
                else
                    if(strcmp(arguments[0],"show") == 0 && (cant > 1))
                        command = SHOW;
                    else
                        if(strcmp(arguments[0],"chperm") == 0 && (cant == 3))
                            command = CHPERM;
                        else
                            if(strcmp(arguments[0],"help") == 0 && (cant == 1))
                                command = HELP;
                            else
                                if(strcmp(arguments[0],"quit") == 0 && (cant == 1))
                                    command = QUIT;
    return command;
}

int main(){
    printf(PRINT_AMARILLO_B"*****************************************************************************************\n");
    printf("**************************************--MINISHELL--**************************************\n");
    printf("******Escriba el comando 'help' para mostrar un listado con los comandos disponibles*****\n");
    printf("*****************************************************************************************\n"PRINT_RESET);
    print_path();    //imprime la ruta junto con el usuario actual
    char command[COM_MAX_SIZE]; //aquí se almacena el command escrito por teclado por el usuario.
    char *arguments[MAX_ARG];    //sirve para almacenar los distintos arguments que hay en el string command.
    for(int i = 0; i < MAX_ARG; i++)
        arguments[i] = (char *)malloc(sizeof(char)*20);
    fflush(stdin); //limpio el buffer de entrada del teclado.
    while(1){   //el programa no termina hasta que el usuario use el command 'salir'
        for(int i = 0; i < MAX_ARG; i++)
            arguments[i] = NULL;   //limpio el arreglo de arguments antes de empezar una nueva iteración.
        int cant = 0;
        scanf("%[^\n]s",command);   //almacena en la variable command todo lo que escriba el usuario hasta que presiona la tecla enter.
        __fpurge(stdin);    //limpio el buffer del teclado.
        pid_t pid;
        cant = get_arguments(command,arguments);   //obtengo la cantida de arguments y los almaceno en la variable 'arguments'.
        int command_id = valid_arguments(arguments,cant);
        if(command_id == ARG_INVALIDOS){
            printf("El comando ingresado es inválido, escriba 'help' para ver la lista de comandos disponibles\n");
            print_path();
        }else{
            if((pid = fork()) < 0)  //creo el proceso que se ocupa de  ejecutar el command recibido por teclado.
                report_and_exit("FORK");
            if(pid == 0){   //estoy en el proceso hijo.
                switch(command_id){
                    case RMDIR:
                        execv("rmdir",arguments);
                        break;
                    case MKDIR:
                        execv("mkdir",arguments);
                        break;
                    case CREATE:
                        execv("create",arguments);
                        break;
                    case DIRLIST:
                        execv("dirlist",arguments);
                        break;
                    case SHOW:
                        execv("show",arguments);
                        break;
                    case CHPERM:
                        execv("chperm",arguments);
                        break;
                    case HELP:
                        execv("help",arguments);
                        break;
                    case QUIT:
                        execv("quit",arguments);
                        break;
                }
            }
        }
        
    }

    return 0;
}
