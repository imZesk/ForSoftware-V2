#include "Pregunta.h"

Pregunta::Pregunta(int id, std::string tipo, std::string pregunta, std::vector<std::string> opciones, std::string respuesta) {
    this->idPregunta = id;
    this->tipoPregunta = tipo;
    this->pregunta = pregunta;
    this->opciones = opciones;
    this->respuesta = respuesta;
}