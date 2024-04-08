#ifndef ENCUESTA_H
#define ENCUESTA_H
#include "GrupoPreguntas.h"

typedef struct encuesta{
    int idEncuesta;
    char nombreEncuesta[50];
	GrupoPreguntas preguntasEncuesta;
}Encuesta;

Encuesta crearEncuesta(GrupoPreguntas preguntasEncuesta, const char *nombreEncuesta);


#endif