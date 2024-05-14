#ifndef ENCUESTA_H
#define ENCUESTA_H

#include <string>
#include <vector>
#include "Pregunta.h"

class Encuesta {
public:
    int id;
    int cantidadPreguntas;
    std::string nombre;
    std::vector<Pregunta> preguntas;

    Encuesta(int id, int cantidadPreguntas, std::string nombre);
    void agregarPregunta(Pregunta pregunta);
};

#endif // ENCUESTA_H