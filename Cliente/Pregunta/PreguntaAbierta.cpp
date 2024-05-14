#include "PreguntaAbierta.h"

PreguntaAbierta::PreguntaAbierta(int id, std::string pregunta, std::string respuesta) : Pregunta(id, pregunta), std::string(respuesta) {}

std::string PreguntaAbierta::obtenerTipo() const {
    return "Verdadero o Falso";
}

PreguntaAbierta:: std::string PreguntaAbierta::obtenerRespuesta() const {
    return respuesta;
}