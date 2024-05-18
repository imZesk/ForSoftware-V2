#include "PreguntaAbierta.h"
#include <string.h>
#include <iostream>
using namespace std;

    PreguntaAbierta::PreguntaAbierta() :Pregunta(){
        respuesta = new char[512];
    }
    PreguntaAbierta::PreguntaAbierta(int i, char *nom, char *r) :Pregunta(i,nom){
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

    int PreguntaAbierta::getTipo(){
        return 1;
    }