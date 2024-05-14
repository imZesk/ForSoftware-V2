#include "PreguntaVerdaderoFalso.h"

PreguntaVerdaderoFalso::PreguntaVerdaderoFalso(int id, std::string pregunta) : Pregunta(id, pregunta) {}

std::string PreguntaVerdaderoFalso::obtenerTipo() const {
    return "Verdadero o Falso";
}