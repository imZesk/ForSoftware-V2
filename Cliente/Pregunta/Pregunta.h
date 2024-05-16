#ifndef PREGUNTA_H
#define PREGUNTA_H

#include <string>
#include <vector>

class Pregunta {
    char id[5];
    char *nombre;

public:
    Pregunta();
    Pregunta(char *id, char *nom);
    Pregunta(const Pregunta &other);

    const char *getId() const;
    char *getNom() const;
    void setId(char *Id);
    void setNom(char *nom);

    virtual ~Pregunta();
};

#endif // PREGUNTA_H