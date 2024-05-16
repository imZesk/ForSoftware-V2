#include "PreguntaOpcionMultiple.h"
#include <iostream>
using namespace std;

    PreguntaOpcionMultiple::PreguntaOpcionMultiple(){
        numOp = 0;
        respuesta = '\0';
    }

    void PreguntaOpcionMultiple::anyadirOpcion(char *opcion, int pos) {
        if (pos >= 0 && pos < 3) {
            opciones[pos] = opcion;
        }
    }

    PreguntaOpcionMultiple::~PreguntaOpcionMultiple(){

    }

