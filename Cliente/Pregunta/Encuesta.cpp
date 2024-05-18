#include "Encuesta.h"
#include <iostream>
#include <string.h>
using namespace std;

Encuesta::Encuesta() {
    cantidadPreguntas = 0;
    preguntas = nullptr;
    notaU = 0;
    numPreguntasAgregadas = 0;
}

Encuesta::Encuesta(int cantPreg) {
    cantidadPreguntas = cantPreg;
    preguntas = new Pregunta*[cantidadPreguntas];
    for (int i = 0; i < cantidadPreguntas; ++i) {
        preguntas[i] = nullptr;
    }
    numPreguntasAgregadas = 0;
}

void Encuesta::agregarPregunta(Pregunta &p) {
    if (numPreguntasAgregadas < cantidadPreguntas) {
        preguntas[numPreguntasAgregadas] = &p;
        numPreguntasAgregadas++;
    } else {
        cout << "No se pueden agregar más preguntas. Se ha alcanzado el límite." << endl;
    }
}

Encuesta::~Encuesta() {
    if (preguntas != nullptr) {
        for (int i = 0; i < numPreguntasAgregadas; ++i) {
            delete preguntas[i];
        }
        delete[] preguntas;
    }
}

void Encuesta::setNombre(const char* nombre) {
    this->nombre = new char[strlen(nombre) + 1];
    strcpy(this->nombre, nombre);
}

void Encuesta::setId(int i) {
    id = i;
}

void Encuesta::setNota(int n) {
    notaU = n;
}

void Encuesta::setCantidadPreguntas(int cantidadPreguntas) {
    this->cantidadPreguntas = cantidadPreguntas;
}

char* Encuesta::getNombre() {
    return nombre;
}

int Encuesta::getId() {
    return id;
}

int Encuesta::getNota() {
    return notaU;
}

int Encuesta::getCantidadPreguntas() {
    return cantidadPreguntas;
}

void Encuesta::reset() {
    if (preguntas != nullptr) {
        for (int i = 0; i < numPreguntasAgregadas; ++i) {
            delete preguntas[i];
        }
        delete[] preguntas;
        preguntas = nullptr;
    }
    cantidadPreguntas = 0;
    numPreguntasAgregadas = 0;
    notaU = 0;
    delete[] nombre;
    nombre = nullptr;
}
