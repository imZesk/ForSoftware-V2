#include "PreguntaVerdaderoFalso.h"
#include <string.h>
#include <iostream>
using namespace std;

    PreguntaVerdaderoFalso::PreguntaVerdaderoFalso() :Pregunta(){
        respuesta = 'F';
    }
    PreguntaVerdaderoFalso::PreguntaVerdaderoFalso(char *i, char *nom, char r) :Pregunta(i,nom){
        respuesta = r;
    }
    PreguntaVerdaderoFalso::PreguntaVerdaderoFalso(const PreguntaVerdaderoFalso &other): Pregunta(other){
        respuesta = other.respuesta;
    }

    char PreguntaVerdaderoFalso::getRespuesta() const{
        return respuesta;
    }
    void PreguntaVerdaderoFalso::setRespuesta(char r){
        respuesta = r;
    }

    PreguntaVerdaderoFalso:: ~PreguntaVerdaderoFalso(){

    }
