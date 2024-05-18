#ifndef PREGUNTAVERDADEROFALSO_H
#define PREGUNTAVERDADEROFALSO_H

#include "Pregunta.h"

class PreguntaVerdaderoFalso : public Pregunta {
private:
    char respuesta;

public:
    PreguntaVerdaderoFalso();
    PreguntaVerdaderoFalso(int i, char *nom, char r);
    PreguntaVerdaderoFalso(const PreguntaVerdaderoFalso &other);

    char getRespuesta() const;
    void setRespuesta(char r);

    int getTipo();

    virtual ~PreguntaVerdaderoFalso();
};

#endif // PREGUNTAVERDADEROFALSO_H