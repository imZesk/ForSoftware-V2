#include <iostream>
#include <cstring>
#include "pregunta.h"

using namespace std;

void responderPregunta(Pregunta pregunta) {
    cout << "Pregunta: " << pregunta.pregunta << endl;
    cout << "Respuesta: ";
    char respuesta[100];
    cin.getline(respuesta, sizeof(respuesta));
    pregunta.respuesta = strdup(respuesta);
    cout << "Respuesta guardada: " << pregunta.respuesta << endl;
}