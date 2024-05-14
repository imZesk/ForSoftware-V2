#ifndef PREGUNTAVERDADEROFALSO_H
#define PREGUNTAVERDADEROFALSO_H

#include "Pregunta/Pregunta.h"

class PreguntaVerdaderoFalso : public Pregunta {
public:
    enum Respuesta { Verdadero, Falso };

    PreguntaVerdaderoFalso(int id, std::string pregunta, Respuesta respuesta);

    std::string obtenerTipo() const override;
    Respuesta obtenerRespuesta() const;

private:
    Respuesta respuesta;
};

#endif // PREGUNTAVERDADEROFALSO_H