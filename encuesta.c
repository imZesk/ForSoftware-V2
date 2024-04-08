#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grupoPreguntas.h"
#include "encuesta.h"

Encuesta crearEncuesta(GrupoPreguntas preguntasEncuesta, const char *nombreEncuesta)
{

    FILE *archivo;                         // Declara un puntero a FILE
    archivo = fopen("./lib/Log.txt", "w"); // Abre el archivo en modo escritura ("w")

    if (archivo == NULL)
    { // Verifica si ocurrió algún error al abrir el archivo
        printf("Error al abrir el archivo.\n");
        Encuesta encuesta_vacia;
        return encuesta_vacia;
    }


    fprintf(archivo, "Nombre de la encuesta: %s. \n", nombreEncuesta);

    Encuesta encuesta;
    strcpy(encuesta.nombreEncuesta, nombreEncuesta);
    encuesta.preguntasEncuesta = preguntasEncuesta;

    return encuesta;
}