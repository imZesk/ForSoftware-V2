#include "Encuesta.h"

Encuesta::Encuesta(int id, int cantidadPreguntas, std::string nombre) {
    this->id = id;
    this->cantidadPreguntas = cantidadPreguntas;
    this->nombre = nombre;
}

void Encuesta::agregarPregunta(Pregunta pregunta) {
    this->preguntas.push_back(pregunta);
    this->cantidadPreguntas++;
}