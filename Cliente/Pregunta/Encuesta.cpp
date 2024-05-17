#include "encuesta.h"
#include <iostream>
#include <string.h>
using namespace std;

Encuesta::Encuesta(){
    cantidadPreguntas = 0;
    preguntas = NULL;
}

Encuesta::Encuesta(int cantPreg){
    cantidadPreguntas = cantPreg;
    preguntas = new Pregunta[cantidadPreguntas];
}

Encuesta Encuesta::agregarPregunta(Encuesta e, Pregunta p){
    Encuesta aux(e.getCantidadPreguntas() + 1);
    for(int i = 0; i < e.getCantidadPreguntas(); i++){
        aux.preguntas[i] = e.preguntas[i];
    }
    aux.preguntas[e.getCantidadPreguntas()] = p;
    return aux;
}

Encuesta::~Encuesta(){
    if(preguntas!=NULL){
		delete [] preguntas;
	}
}

void Encuesta::setNombre(char* nombre){
	this->nombre = new char[strlen(nombre) + 1];
	strcpy(this->nombre, nombre);
}

void Encuesta::setId(int i){
    id = i;
}

void Encuesta::setCantidadPreguntas(int cantidadPreguntas){
	this->cantidadPreguntas = cantidadPreguntas;
}

char* Encuesta::getNombre(){
	return nombre;
}

int Encuesta::getId(){
    return id;
}

int Encuesta::getCantidadPreguntas(){
	return cantidadPreguntas;
}