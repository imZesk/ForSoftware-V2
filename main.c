/*
1. crear test
    1.1 tipo pregunta: (T o F), (a,b,c,d), (a,b,c,d *2T), pregunta corta(limite de x caracteres), pregunta abierta
    1.2 continuar
    1.3 finalizar
2. hacer test
    tests:
        1. test1
        2. test2...
    2.1 hacer test x

    2.2 atras
3. visualizar nota
    tests:
        test1 3/5
        test2 8/10
        ...

4. eliminar test
    tests...
    4.1 elimanr test x
        4.1.1 confirmar test x?
        4.1.2 cancelar
    4.2 atras
5. salir
*/

#include <sqlite3.h>
#include <stdio.h>

int main(){

    sqlite3 *DB;

    //Abrimos la bd
    int existe = sqlite3_open("./ruta.db", &DB);

    //Confirmamos que se abre correctamente
    if (existe != SQLITE_OK) {
        printf("Error");
        //logger con el error
        return 1;
    }

    //avisamos por consola que la base de datos se ha abierto correctamente
    printf("Conexión exitosa a la base de datos");

    //creamos la tabla
    string sql = "CREATE TABLE IF NOT EXISTS PREGUNTA(
                ID INTEGER PRIMARY KEY AUTOINCREMENT,
                PREGUNTA TEXT NOT NULL
                )"; // resto de parametros de pregunta

    

    sqlite3_close(DB);
    return 0;
}