#include "Pregunta/Pregunta.h"
#include <string.h>
#include <iostream>
using namespace std;

    Pregunta::Pregunta() {
        id[0] = '\0';
        nombre = new char[1];
        nombre[0] = '\0';
    }

    Pregunta::Pregunta(char *i, char *nom){
        strcpy(id,i);
        nombre = new char[strlen(nom)+1];
        strcpy(nombre, nom);
    }

    Pregunta::Pregunta(const Pregunta &other){
        strcpy(id, other.id);
        nombre = new char[strlen(other.nombre)+1];
        strcpy(nombre, other.nombre);
    }

    const char * Pregunta::getId() const{
        return id;
    }
    char * Pregunta::getNom() const{
        return nombre;
    }
    void Pregunta::setId(char *i){
        strcpy(id, i);
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

