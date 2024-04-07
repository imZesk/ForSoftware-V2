#include "menus.h"
#include <stdio.h>
/*
1. crear test
    1.1 tipo pregunta: (T o F), (a,b,c,d), (a,b,c,d *2T), pregunta corta(limite de x caracteres), pregunta abierta
    1.2 continuar
    1.3 finalizar
2. hacer test
    tests:
        1. test1
        2. test2...
    2.1 hacer test x

    2.2 atras
3. visualizar nota
    tests:
        test1 3/5
        test2 8/10
        ...

4. eliminar test
    tests...
    4.1 elimanr test x
        4.1.1 confirmar test x?
        4.1.2 cancelar
    4.2 atras
5. salir
*/


char menuPrincipal(){
	char opcion;
	printf("1. Crear Test\n");
	printf("2. Realizar Test\n");
	printf("3. Visualizar Nota\n");
	printf("4. Eliminar Test\n");
	printf("0. Salir\n");
	printf("Elige una opcion: ");
    
	fflush(stdout);
	fflush(stdin);
	
	scanf("%c",&opcion);
    
	return opcion;
}

char menuSecundario1(){
	char opcion;
	printf("1. \n");
	printf("2. \n");
	printf("0. Salir\n");
	printf("Elige una opcion: ");

	scanf("%c",&opcion);
	return opcion;
}

char menuSecundario2(){
	char opcion;
	printf("1. \n");
	printf("0. Salir\n");
	printf("Elige una opcion: ");

	scanf("%c",&opcion);
	return opcion;
}


