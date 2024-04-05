#ifndef GRUPO_PREGUNTAS_H
#define GRUPO_PREGUNTAS_H
#include "Pregunta.h"

typedef struct grupoPreguntas{
	Pregunta * arrPreguntas;
	int tam;
	int numPreguntas;
}GrupoPreguntas;

grupoPreguntas reservarMemoria(int tam);

void addPregunta(GrupoPreguntas * grupo, Pregunta pregunta);

#endif // GRUPO_PREGUNTAS_H
