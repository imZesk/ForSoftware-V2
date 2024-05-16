#include "Encuesta.h"
#include <iostream>
#include <string.h>
using namespace std;

Encuesta::Encuesta(){
    cantidadPreguntas = 0;
    preguntas = NULL;
}

void Encuesta::agregarPregunta(const Pregunta &p){
    	if(preguntas == NULL){
		preguntas = new Pregunta[1];
	}else{
		//Reservo memoria para un array auxiliar
		Pregunta *aux = new Pregunta[cantidadPreguntas];
		//Copio los datos de la lista al array auxiliar
		for(int i=0;i<cantidadPreguntas;i++){
			aux[i] = preguntas[i]; //Como son objetos, sobrecarga del operador = en la clase Registro
		}
		//Libero la memoria de la lista
		delete[] preguntas;
		//Volvemos a reservar con un posici�n m�s
		preguntas = new Pregunta[cantidadPreguntas+1];
		//Copiamos del array auxiliar a nuestra lista
		for(int i=0;i<cantidadPreguntas;i++){
			preguntas[i] = aux[i];
		}
		//Liberamos el array auxiliar
		delete [] aux;
	}
	preguntas[cantidadPreguntas] = p;
	cantidadPreguntas++;
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