#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "pregunta.h"

void responderPregunta(Pregunta pregunta){
    printf("Pregunta: %s\n", pregunta.pregunta);
    printf("Respuesta: ");
    char respuesta[100];
    fgets(respuesta, sizeof(respuesta), stdin);
    respuesta[strcspn(respuesta, "\n")] = '\0';
    pregunta.respuesta = strdup(respuesta);
    printf("Respuesta guardada: %s\n", pregunta.respuesta);
}