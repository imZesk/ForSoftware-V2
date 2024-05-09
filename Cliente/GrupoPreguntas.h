#ifndef GRUPO_PREGUNTAS_H
#define GRUPO_PREGUNTAS_H
#include "Pregunta.h"

typedef struct grupoPreguntas{
	Pregunta * arrPreguntas;
	char nombreEncuesta[50];
	int tam;
	int numPreguntas;
}GrupoPreguntas;

GrupoPreguntas reservarMemoria();

void addPregunta(GrupoPreguntas *grupo);
void mostarPreguntas(GrupoPreguntas grupo);

#endif // GRUPO_PREGUNTAS_H
