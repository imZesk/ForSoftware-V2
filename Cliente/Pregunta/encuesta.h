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
    Encuesta(int cantPreg);
    Encuesta agregarPregunta(Encuesta e, Pregunta p);

    void setNombre(const char* nombre);
    void setCantidadPreguntas(int cantidadPreguntas);
    char* getNombre();
    int getCantidadPreguntas();
    
    Pregunta& operator=(const Pregunta &p);

    virtual ~Encuesta();
};

#endif // ENCUESTA_H