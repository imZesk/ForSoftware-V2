#ifndef PREGUNTAVERDADEROFALSO_H
#define PREGUNTAVERDADEROFALSO_H

#include "Pregunta/Pregunta.h"

class PreguntaVerdaderoFalso : public Pregunta {
private:
    char respuesta;

public:
    PreguntaVerdaderoFalso();
    PreguntaVerdaderoFalso(char *i, char *nom, char r);
    PreguntaVerdaderoFalso(const PreguntaVerdaderoFalso &other);

    char getRespuesta() const;
    void setRespuesta(char r);

    int getTipo();

    virtual ~PreguntaVerdaderoFalso();
};

#endif // PREGUNTAVERDADEROFALSO_H