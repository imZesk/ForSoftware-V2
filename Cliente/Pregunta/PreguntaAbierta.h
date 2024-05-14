#ifndef PREGUNTAABIERTA_H
#define PREGUNTAABIERTA_H

#include "Pregunta/Pregunta.h"

class PreguntaAbierta : public Pregunta {
    char *respuesta;
public:
    PreguntaAbierta();
    PreguntaAbierta()

    PreguntaAbierta(int id, std::string pregunta, std::string respuesta);

    std::string obtenerTipo() const override;
    std::string obtenerRespuesta() const;

private:
    std::string respuesta;
};

#endif // PREGUNTAVERDADEROFALSO_H