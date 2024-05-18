#include "menus.h"
#include <iostream>

using namespace std;

char menuPrincipal()
{
    char opcion;
    cout << "1. Crear Test"<<endl;
    cout << "2. Realizar Test"<<endl;
    cout << "3. Visualizar Teses"<<endl;
    cout << "4. Eliminar Test"<<endl;
    cout << "5. Agregar Pregunta"<<endl;
    cout << "6. Resultado teses"<<endl;
    cout << "0. Salir"<<endl;
    cout << "Elige una opcion: ";

    cin >> opcion;

    return opcion;
}

char menuSecundario1()
{
    char opcion;
    cout << "1. "<<endl;
    cout << "2. "<<endl;
    cout << "0. Salir"<<endl;
    cout << "Elige una opcion: ";

    cin >> opcion;
    return opcion;
}

char menuSecundario2()
{
    char opcion;
    cout << "1. "<<endl;
    cout << "0. Salir"<<endl;
    cout << "Elige una opcion: ";

    cin >> opcion;
    return opcion;
}