#include "PreguntaAbierta.h"
#include <string.h>
#include <iostream>
using namespace std;

    PreguntaAbierta::PreguntaAbierta() :Pregunta(){
        respuesta = "";
    }
    PreguntaAbierta::PreguntaAbierta(char *i, char *nom, char *r) :Pregunta(i,nom){
        respuesta = r;
    }
    PreguntaAbierta::PreguntaAbierta(const PreguntaAbierta &other): Pregunta(other){
        respuesta = other.respuesta;
    }

    char* PreguntaAbierta::getRespuesta() const{
        return respuesta;
    }
    void PreguntaAbierta::setRespuesta(char *r){
        strcpy(respuesta,r);
    }

    PreguntaAbierta:: ~PreguntaAbierta(){

    }
