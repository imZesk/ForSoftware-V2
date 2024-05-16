#include "Encuesta.h"
#include <iostream>
#include <string.h>
using namespace std;

Encuesta::Encuesta(){
    cantidadPreguntas = 0;
    numPreg = 0;
    preguntas = NULL;
}

Encuesta::Encuesta(int cantPreg){
    cantidadPreguntas = cantPreg;
    numPreg = 0;
    preguntas = new Pregunta[cantidadPreguntas];
}

void Encuesta::agregarPregunta(const Pregunta &p){
    if(numPreg < cantidadPreguntas){
		preguntas[numPreg] = p; 
		numPreg++;
    }
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

void Encuesta::setCantidadPreguntas(int cantidadPreguntas){
	this->cantidadPreguntas = cantidadPreguntas;
}

char* Encuesta::getNombre(){
	return nombre;
}

int Encuesta::getCantidadPreguntas(){
	return cantidadPreguntas;
}

Encuesta crearEncuesta(char *nombre, int cantidadPreguntas){
	Encuesta encuesta;
	encuesta.setNombre(nombre);
	encuesta.setCantidadPreguntas(cantidadPreguntas);
	return encuesta;
}