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

    int PreguntaOpcionMultiple::getTipo(){
        return 2;
    }

    char* PreguntaOpcionMultiple::getRespuesta() {
        return respuesta;
    }

    void PreguntaOpcionMultiple::setRespuesta(char* respuesta) {
        this->respuesta = respuesta;
    }

    int PreguntaOpcionMultiple::getNumOp() {
        return numOp;
    }

    void PreguntaOpcionMultiple::setNumOp(int numOp) {
        this->numOp = numOp;
    }

    char* PreguntaOpcionMultiple::getOpcion(int pos) {
        if (pos >= 0 && pos < 3)
            return opciones[pos];
        else
            return nullptr; 
    }

    void PreguntaOpcionMultiple::setOpcion(char* opcion, int pos) {
        if (pos >= 0 && pos < 3)
            opciones[pos] = opcion;
        else
            std::cout << "Posición fuera de rango.";
    }

    PreguntaOpcionMultiple::~PreguntaOpcionMultiple(){

    }