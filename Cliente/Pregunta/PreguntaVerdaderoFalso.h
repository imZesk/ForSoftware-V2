#ifndef PREGUNTAVERDADEROFALSO_H
#define PREGUNTAVERDADEROFALSO_H

#include "Pregunta.h"

class PreguntaVerdaderoFalso : public Pregunta {
public:
    PreguntaVerdaderoFalso(int id, std::string pregunta);
    std::string obtenerTipo() const override;
};

#endif // PREGUNTAVERDADEROFALSO_H