#ifndef PREGUNTAOPCIONMULTIPLE_H
#define PREGUNTAOPCIONMULTIPLE_H

#include "Pregunta.h"
#include <vector>

class PreguntaOpcionMultiple : public Pregunta {
private:
    char* opciones[3];
    char* respuesta;
    int numOp;
public:
    PreguntaOpcionMultiple();
    void anyadirOpcion(char *opcion, int pos);

    
    int getTipo();

    char* getRespuesta();
    void setRespuesta(char* respuesta);

    int getNumOp();
    void setNumOp(int numOp);

    char* getOpcion(int pos);
    void setOpcion(char* opcion, int pos);

    virtual ~PreguntaOpcionMultiple();
};

#endif // PREGUNTAOPCIONMULTIPLE_H