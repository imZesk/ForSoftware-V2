#include "grupoPreguntas.h"
#include "pregunta.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void addPregunta(GrupoPreguntas * grupo){

    Pregunta pregunta;

    char preguntaText[100];
    char tipoPreguntaText[100];

    printf("Ingrese la pregunta: ");
    fgets(preguntaText, sizeof(preguntaText), stdin);

    printf("Ingrese el tipo de pregunta: ");
    fgets(tipoPreguntaText, sizeof(tipoPreguntaText), stdin);

    preguntaText[strcspn(preguntaText, "\n")] = '\0';
    tipoPreguntaText[strcspn(tipoPreguntaText, "\n")] = '\0';

    pregunta.pregunta = strdup(preguntaText);
    pregunta.tipoPregunta = strdup(tipoPreguntaText);

    if(grupo->numPreguntas < grupo->tam){
        grupo->arrPreguntas[grupo->numPreguntas] = pregunta;
        grupo->numPreguntas++;
        printf("Pregunta añadida correctamente\n");
    }else{
        printf("No se pueden añadir más preguntas\n");
    }
}

void mostarPreguntas(GrupoPreguntas grupo){
    for(int i = 0; i < grupo.numPreguntas; i++){
        printf("Pregunta %d: %s\n", i+1, grupo.arrPreguntas[i].pregunta);
    }
}