#ifndef SERVER_H_
#define SERVER_H_
#include <iostream>
#include <string>
#include <sqlite3.h>

using namespace std;

namespace containers
{
    class BD
    {
    private:
    public:
        void DB(sqlite3* BD);
        void desconectar(sqlite3* BD);
        void realizarTest(char nombreTest);
        void visualizarTest(int existe);
        void eliminarTest(char nombreTest);
        void crearTest();
        void añadirPregunta(char nombreTest);
        void visualizarNota();
    };
    
}

#endif