#ifndef PREGUNTAOPCIONMULTIPLE_H
#define PREGUNTAOPCIONMULTIPLE_H

#include "Pregunta.h"
#include <vector>

class PreguntaOpcionMultiple : public Pregunta {
public:
    PreguntaOpcionMultiple(int id, std::string pregunta, std::vector<std::string> opciones, int opcionCorrecta);

    std::string obtenerTipo() const override;
    std::vector<std::string> obtenerOpciones() const;
    int obtenerOpcionCorrecta() const;

private:
    std::vector<std::string> opciones;
    int opcionCorrecta;
};

#endif // PREGUNTAOPCIONMULTIPLE_H