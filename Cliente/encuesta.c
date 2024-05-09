#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grupoPreguntas.h"
#include "encuesta.h"
#include <time.h>

Encuesta crearEncuesta(GrupoPreguntas preguntasEncuesta, const char *nombreEncuesta)
{
    time_t hora = time(NULL);
    struct tm tm = *localtime(&hora);
    FILE *archivo;                         // Declara un puntero a FILE
    archivo = fopen("./lib/Log.txt", "w"); // Abre el archivo en modo escritura ("w")

    if (archivo == NULL)
    { // Verifica si ocurrió algún error al abrir el archivo
        printf("Error al abrir el archivo.\n");
        Encuesta encuesta_vacia;
        return encuesta_vacia;
    }


    fprintf(archivo, "[%d:%d:%d] Nombre de la encuesta: %s. \n",tm.tm_hour,tm.tm_min,tm.tm_sec, nombreEncuesta);

    Encuesta encuesta;
    strcpy(encuesta.nombreEncuesta, nombreEncuesta);
    encuesta.preguntasEncuesta = preguntasEncuesta;

    return encuesta;
}