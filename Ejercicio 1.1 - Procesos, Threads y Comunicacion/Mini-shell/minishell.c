/* 
 * Sistemas operativos 2021 
 * Almaraz Fabricio, Buñes Juan.
 * 
 * */
#include "shared.h"

/* Retorna la cantidad de argumentos leidos en el comando pasado por parámetro y     
   almacena dichos argumentos en un arreglo de strings.
*/
int get_arguments(char *command, char *arguments []){
    int cont = 0;
    arguments[cont] = strtok(command," ");   // almacena en el primer elemento el nombre del comando.
    while(arguments[cont] != NULL && cont < MAX_ARG){    // mientras que haya elementos y no sean > 4.
        cont++;
        arguments[cont] = strtok(NULL," ");    // almacena el siguiente argumento del comando.
    }
    return cont;
}

/* Analiza los argumentos ingresados y retorna el identificador del comando correspondiente.
   Si los argumentos no son validos, retorna -1 */
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
    printf("                                        --MINISHELL--\n");
    printf("        Escriba el comando 'help' para mostrar un listado con los comandos disponibles\n");
    printf(" \n\n\n"PRINT_RESET);
    print_path();    // Imprime la ruta junto con el usuario actual
    char command[COM_MAX_SIZE]; // Aqui se almacena el comando escrito por teclado por el usuario.
    char *arguments[MAX_ARG];    //sirve para almacenar los distintos arguments que hay en el string comando.

    for(int i = 0; i < MAX_ARG; i++)
        arguments[i] = (char *)malloc(sizeof(char)*20);
    fflush(stdin); //limpio el buffer de entrada del teclado.

    while(1){   // el programa termina cuando el usuario usa el comando 'quit'
        for(int i = 0; i < MAX_ARG; i++)
            arguments[i] = NULL;   // se vacía el arreglo
        int cant = 0;
        scanf("%[^\n]s",command);   //almacena en la variable command todo lo que escriba el usuario hasta que presiona la tecla enter.
        __fpurge(stdin);    // limpio el buffer del teclado.
        pid_t pid;
        cant = get_arguments(command,arguments);
        int command_id = valid_arguments(arguments,cant);
        if(command_id == ARG_INVALIDOS){
            printf("El comando ingresado es inválido, escriba 'help' para ver la lista de comandos disponibles\n");
            print_path();
        }else{
            if((pid = fork()) < 0)  // creo el proceso que se ocupa de  ejecutar el comando recibido por teclado.
                report_and_exit("FORK");
            if(pid == 0){   
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
