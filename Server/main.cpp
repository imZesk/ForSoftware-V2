#include <iostream>
#include "./Sqlite3/sqlite3.h"
using namespace std;

int conectar(sqlite3 *BD)
{
    int existe = sqlite3_open("./lib/servidor.bd", &BD);
    if (existe != SQLITE_OK)
    {
        cout << "Error al abrir la base de datos" << endl;
    }
    return existe;
};

int main()
{
    sqlite3 *bd;
    int existe = conectar(bd);
    cout<<existe<<endl;
}