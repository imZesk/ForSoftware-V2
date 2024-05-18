#ifndef PREGUNTA_H
#define PREGUNTA_H

#include <string>
#include <vector>

class Pregunta {
    int id;
    char *nombre;

public:
    Pregunta();
    Pregunta(int id, char *nom);
    Pregunta(const Pregunta &other);

    const int getId() const;
    char *getNom() const;
    void setId(int Id);
    void setNom(char *nom);

    virtual ~Pregunta();
};

#endif // PREGUNTA_H