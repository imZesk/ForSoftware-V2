#ifndef ENCUESTA_H
#define ENCUESTA_H

#include "Pregunta.h"
#include <vector>

class Encuesta {
private:
    int id;
    int cantidadPreguntas;
    int numPreg;
    char* nombre;
    Pregunta *preguntas;

public:
    Encuesta();
    void agregarPregunta(const Pregunta &p);

    void setNombre(char* nombre);
    void setCantidadPreguntas(int cantidadPreguntas);
    char* getNombre();
    int getCantidadPreguntas();
    
    Encuesta crearEncuesta(char *nombre, int cantidadPreguntas);


    virtual ~Encuesta();
};

#endif // ENCUESTA_H