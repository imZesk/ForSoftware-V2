#ifndef PREGUNTA_H
#define PREGUNTA_H

#include <string>
#include <vector>

class Pregunta {
public:
    int idPregunta;
    std::string tipoPregunta;
    std::string pregunta;
    std::vector<std::string> opciones;
    std::string respuesta;

    Pregunta(int id, std::string tipo, std::string pregunta, std::vector<std::string> opciones, std::string respuesta);
};

#endif // PREGUNTA_H