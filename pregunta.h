#ifndef PREGUNTA_H
#define PREGUNTA_H

typedef struct {
    char *pregunta;
    char *tipoPregunta;
    char *respuesta;
}Pregunta;

void responderPregunta(Pregunta pregunta);

#endif // PREGUNTA_H