// Programa que recibe como entrada una letra (-a, -p). Dependiendo de la letra se calculara el area o perimetro de un rectangulo con los datos ingresados.
// Ejmplo: rectangle -a b 10 h 20, donde b es base y h es altura

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

    // Declaracion de variables
    int area, perimetro, base, altura;
    const char *a = "-a";
    const char *b = "-p";
    int opArea = strcmp(a, argv[1]);
    int opPerimetro = strcmp(b, argv[1]);

    // Operaciones
    if (opArea == 0){

        base = atoi(argv[3]);
        altura = atoi(argv[5]);
        area = base * altura;
        printf("Area: %d unidades\n", area); 

    } else if (opPerimetro == 0){

        base = atoi(argv[3]);
        altura = atoi(argv[5]);
        perimetro = (2*base) + (2*altura);
        printf("Perimetro: %d unidades\n", perimetro);  

    } else {

        printf("Opcion no permitida.\n");

    }   

    return 0;
}