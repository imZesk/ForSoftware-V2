#include "PreguntaVerdaderoFalso.h"

PreguntaVerdaderoFalso::PreguntaVerdaderoFalso(int id, std::string pregunta, Respuesta respuesta) : Pregunta(id, pregunta), respuesta(respuesta) {}

std::string PreguntaVerdaderoFalso::obtenerTipo() const {
    return "Verdadero o Falso";
}

PreguntaVerdaderoFalso::Respuesta PreguntaVerdaderoFalso::obtenerRespuesta() const {
    return respuesta;
}