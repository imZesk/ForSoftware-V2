#include "Pregunta.h"
#include <string.h>
#include <iostream>
using namespace std;

    Pregunta::Pregunta() {
        id = 0;
        nombre = new char[1];
        nombre[0] = '\0';
    }

    Pregunta::Pregunta(int i, char *nom){
        id = i;
        nombre = new char[strlen(nom)+1];
        strcpy(nombre, nom);
    }

    Pregunta::Pregunta(const Pregunta &other){
        id = other.id;
        nombre = new char[strlen(other.nombre)+1];
        strcpy(nombre, other.nombre);
    }

    const int Pregunta::getId() const{
        return id;
    }
    char * Pregunta::getNom() const{
        return nombre;
    }
    void Pregunta::setId(int i){
        id = i;
    }
    void Pregunta::setNom(char *nom){
        delete [] nombre;
        nombre = new char[strlen(nom)+1];
        strcpy(nombre, nom);
    }

    //funciones


    Pregunta::~Pregunta(){
        delete [] nombre;
    }

