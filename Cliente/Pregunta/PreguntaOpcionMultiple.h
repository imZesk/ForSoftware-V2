#ifndef PREGUNTAOPCIONMULTIPLE_H
#define PREGUNTAOPCIONMULTIPLE_H

#include "Pregunta.h"
#include <vector>

class PreguntaOpcionMultiple : public Pregunta {
private:
    char* opciones[3];
    char* respuesta;
public:
    PreguntaOpcionMultiple();
    PreguntaOpcionMultiple(char *i, char *nom, char **opciones);
    PreguntaOpcionMultiple();


};

#endif // PREGUNTAOPCIONMULTIPLE_H