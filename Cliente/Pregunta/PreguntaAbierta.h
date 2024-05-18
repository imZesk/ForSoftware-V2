#ifndef PreguntaAbierta_H
#define PreguntaAbierta_H

#include "Pregunta.h"

class PreguntaAbierta : public Pregunta {
private:
    char *respuesta;

public:
    PreguntaAbierta();
    PreguntaAbierta(int i, char *nom, char *r);
    PreguntaAbierta(const PreguntaAbierta &other);

    char* getRespuesta() const;
    void setRespuesta(char *r);

    int getTipo();

    virtual ~PreguntaAbierta();
};

#endif // PreguntaAbierta_H