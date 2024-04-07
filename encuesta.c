#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grupoPreguntas.h"
#include "encuesta.h"

Encuesta crearEncuesta(GrupoPreguntas preguntasEncuesta){
    char nombreEncuesta[100];
    printf("Introduce el nombre de la encuesta: ");
    scanf("%s", nombreEncuesta);
    
    Encuesta encuesta;
    strcpy(encuesta.nombreEncuesta, nombreEncuesta);
    encuesta.preguntasEncuesta = preguntasEncuesta;
    
    return encuesta;
}