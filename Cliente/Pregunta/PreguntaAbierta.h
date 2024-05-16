#ifndef PreguntaAbierta_H
#define PreguntaAbierta_H

#include "Pregunta/Pregunta.h"

class PreguntaAbierta : public Pregunta {
private:
    char *respuesta;

public:
    PreguntaAbierta();
    PreguntaAbierta(char *i, char *nom, char *r);
    PreguntaAbierta(const PreguntaAbierta &other);

    char* getRespuesta() const;
    void setRespuesta(char *r);

    virtual ~PreguntaAbierta();
};

#endif // PreguntaAbierta_H