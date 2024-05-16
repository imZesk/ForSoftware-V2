#ifndef ENCUESTA_H
#define ENCUESTA_H

#include "Pregunta.h"
#include <vector>

class Encuesta {
private:
    int id;
    int cantidadPreguntas;
    char* nombre;
    Pregunta *preguntas;

public:
    Encuesta();
    void agregarPregunta(const Pregunta &p);

    virtual ~Encuesta();
};

#endif // ENCUESTA_H