#include <stdio.h>
#include "menus.h"
#include "Sqlite3/sqlite3.h"
#include "GrupoPreguntas.h"
#include "encuesta.h"
#include <time.h>

static int callback(void *data, int argc, char **argv, char **azColName)
{
    for (int i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int main()
{


    sqlite3 *DB;
    char *errMsg = 0;

    // Abrimos la bd

    int existe = sqlite3_open("./lib/Preguntas.db", &DB);
    // //Abrimos la bd
    // int existe = sqlite3_open("./lib/Preguntas.db", &DB);

    // //Confirmamos que se abre correctamente
    if (existe != SQLITE_OK)
    {
        printf("Error");
        // logger con el error
            return 1;
    }
    //realizar test
    char sql1[1000];
    //ver test
    char *sql2 = "SELECT nombre, pregunta, respuesta FROM test;";
    //eliminar test
    char sql3[1000];
    //ver nombres
    char *sql4 = "SELECT nombre FROM test;";
    //ver la respuesta
    char sql5[1000];

    //visualizar nota
    char *sql6 = "Select respuesta, preguntas_realizadas from resultado";
    //añadir pregunta a test
    char sql7[1000];
    //visualizar preguntas
    char sql8 = "SELECT * from preguntas";


    existe = sqlite3_exec(DB, sql4, callback, 0, &errMsg);

            if (existe != SQLITE_OK)
            {
                //fprintf(stderr, "[%d:%d:%d] Error en la consulta SQL: %s\n", tm.tm_hour,tm.tm_min,tm.tm_sec, errMsg);
                sqlite3_free(errMsg);
            }
            printf("Nombre del test a hacer: ");

            fflush(stdout);
            fflush(stdin);

            //sprintf(sql1, "SELECT pregunta FROM pregunta WHERE nombre LIKE '%s';", hacer);

            existe = sqlite3_exec(DB, sql1, callback, 0, &errMsg);

            if (existe != SQLITE_OK)
            {
                sqlite3_free(errMsg);
            }
            printf("Responda la pregunta: ");

            fflush(stdout);
            fflush(stdin);


            //sprintf(sql5, "SELECT respuesta FROM pregunta WHERE nombre LIKE '%s';", hacer);

            existe = sqlite3_exec(DB, sql5, callback, 0, &errMsg);

            if (existe != SQLITE_OK)
            {
                   sqlite3_free(errMsg);
            }

            existe = sqlite3_exec(DB, sql2, callback, 0, &errMsg);

            if (existe != SQLITE_OK)
            {
                
                sqlite3_free(errMsg);
            }
            existe = sqlite3_exec(DB, sql4, callback, 0, &errMsg);

            if (existe != SQLITE_OK)
            {
                sqlite3_free(errMsg);
            }
            printf("Nombre del test a eliminar: ");

            fflush(stdout);
            fflush(stdin);

            //sprintf(sql3, "DELETE FROM pregunta WHERE nombre = '%s';", eliminar);

            existe = sqlite3_exec(DB, sql3, 0, 0, NULL);
            if (existe != SQLITE_OK)
            {
                sqlite3_free(errMsg);
            }

            
    sqlite3_close(DB);
    
}