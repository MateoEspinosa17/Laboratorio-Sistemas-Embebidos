// Programa que recibe como entrada -s y la matrìcula y nombr4e de un estudiante, tambien la cantidad de estudiantes que se registraran y se creara un archivo .txt

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[]){

    char id[20], nombre[20], enter[] = "\n", opcion[1], maxString[1];     // Los inicializamos en 20 ya que es el numero maximo de caracteres que se pueden tener
    int max = atoi(argv[2]);

    const char *opcionSi = "s";
    const char *opcionNo = "n";
    const char *comparador = "-s";
    int test = strcmp(comparador, argv[1]);     // Comparamos si el primer argumento ingresado es -s

    bool add = true;

    FILE *file;
    file = fopen("estudiantes.txt", "w");

    if(test == 0){

        while(add){
            
            for(int i = 0; i < max; i++){

                printf("Ingrese el numero de matricula: ");
                scanf("%s", id);
                printf("Ingrese el nombre del estudiante: ");
                scanf("%s", nombre);
                fprintf(file,"%s    %s\n",id , nombre);
            }

            printf("Deseas anadir mas estudiantes [s/n]: ");
            scanf("%s", opcion);

            if(strcmp(opcion, opcionSi) == 0){

                printf("Ingresa el numero de estudiantes que deseas anadir: ");
                scanf("%s", maxString);
                max = atoi(maxString);
                add = true;

            } else {

                printf("Informacion de estudiantes guardada en estudiantes.txt");
                fclose(file);
                add = false;

            }

        }

    } else {

        printf("Opcion no valida");

    }

    return 0;
}