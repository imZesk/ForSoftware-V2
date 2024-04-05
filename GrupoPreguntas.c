#include "grupoPreguntas.h"
#include "pregunta.h"
#include <stdio.h>
#include <stdlib.h>

// typedef struct {
//     char *pregunta;
//     char *tipoPregunta;
//     char *respuesta;
// }Pregunta;

GrupoPreguntas reservarMemoria(int tam){
    GrupoPreguntas grupo;
    grupo.arrPreguntas = (Pregunta *)malloc(tam * sizeof(Pregunta));
    grupo.tam = tam;
    grupo.numPreguntas = 0;
    return grupo;
}

void addPregunta(GrupoPreguntas * grupo, Pregunta pregunta){
    if(grupo->numPreguntas < grupo->tam){
        grupo->arrPreguntas[grupo->numPreguntas] = pregunta;
        grupo->numPreguntas++;
        printf("Pregunta añadida correctamente\n");
    }else{
        printf("No se pueden añadir más preguntas\n");
    }
}