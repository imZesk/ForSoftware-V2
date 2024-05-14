#ifndef PREGUNTA_H
#define PREGUNTA_H

#include <string>
#include <vector>

class Pregunta {
public:
    int idPregunta;
    std::string pregunta;

    Pregunta(int id, std::string pregunta);
    virtual ~Pregunta() = default;

    // Método virtual puro que debe ser implementado por las subclases
    virtual std::string obtenerTipo() const = 0;
};

#endif // PREGUNTA_H