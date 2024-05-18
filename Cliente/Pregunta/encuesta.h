#ifndef ENCUESTA_H
#define ENCUESTA_H

#include "Pregunta.h"

class Encuesta {
private:
    int id;
    char *nombre;
    int cantidadPreguntas;
    Pregunta **preguntas;
    int notaU;
    int numPreguntasAgregadas; 

public:
    Encuesta();
    Encuesta(int cantPreg);
    void agregarPregunta(Pregunta &p); 
    ~Encuesta();

    void setNombre(const char* nombre);
    void setId(int i);
    void setNota(int n);
    void setCantidadPreguntas(int cantidadPreguntas);

    char* getNombre();
    int getId();
    int getNota();
    int getCantidadPreguntas();

    void reset();
};

#endif // ENCUESTA_H
