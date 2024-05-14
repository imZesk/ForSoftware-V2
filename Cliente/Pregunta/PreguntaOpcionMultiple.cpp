#include "PreguntaOpcionMultiple.h"

PreguntaOpcionMultiple::PreguntaOpcionMultiple(int id, std::string pregunta, std::vector<std::string> opciones, int opcionCorrecta) : Pregunta(id, pregunta), opciones(opciones), opcionCorrecta(opcionCorrecta) {}

std::string PreguntaOpcionMultiple::obtenerTipo() const {
    return "Opcion Multiple";
}

std::vector<std::string> PreguntaOpcionMultiple::obtenerOpciones() const {
    return opciones;
}

int PreguntaOpcionMultiple::obtenerOpcionCorrecta() const {
    return opcionCorrecta;
}