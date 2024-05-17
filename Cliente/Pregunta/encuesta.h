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

    void setNombre(char* nombre);
    void setId(int i);
    void setCantidadPreguntas(int cantidadPreguntas);
    char* getNombre();
    int getId();
    int getCantidadPreguntas();
    
    Pregunta& operator=(const Pregunta &p);

    virtual ~Encuesta();
};

#endif // ENCUESTA_H