#include "Server.h"
#include <iostream>
#include <string>
#include <sqlite3.h>

using namespace std;
namespace containers{
    void BD::DB(){
        
    }
    int BD::conectar(sqlite3* BD){
        int existe = sqlite3_open("./lib/servidor.bd",& BD);
        if (existe!=SQLITE_OK){
            cout<<"Error al abrir la base de datos"<<endl;
        }
        return existe;
    };
    void BD::desconectar(sqlite3* BD){
        sqlite3_close(BD);
    };
    void BD::visualizarTest(int existe){
        char *sql = "Select nombre from test;";
        
    }
}