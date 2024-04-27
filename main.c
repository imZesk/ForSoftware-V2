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
    time_t hora = time(NULL);
    struct tm tm = *localtime(&hora);
    FILE *archivo;                         // Declara un puntero a FILE
    archivo = fopen("./lib/Log.txt", "w"); // Abre el archivo en modo escritura ("w")

    if (archivo == NULL)
    { // Verifica si ocurrió algún error al abrir el archivo
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    fprintf(archivo, "[%d:%d:%d] Archivo creado. \n", tm.tm_hour,tm.tm_min,tm.tm_sec);

    sqlite3 *DB;
    char *errMsg = 0;

    // Abrimos la bd

    int existe = sqlite3_open("./lib/Preguntas.db", &DB);
    fprintf(archivo, "[%d:%d:%d] Base de datos abierta. \n", tm.tm_hour,tm.tm_min,tm.tm_sec);
    // //Abrimos la bd
    // int existe = sqlite3_open("./lib/Preguntas.db", &DB);

    // //Confirmamos que se abre correctamente
    if (existe != SQLITE_OK)
    {
        printf("Error");
        // logger con el error
        fprintf(archivo, "[%d:%d:%d] Error al abrir la base de datos. \n", tm.tm_hour,tm.tm_min,tm.tm_sec);
        return 1;
    }
    char sql1[1000];
    char *sql2 = "SELECT nombre, pregunta, respuesta FROM pregunta;";
    char sql3[1000];
    char *sql4 = "SELECT nombre FROM pregunta;";
    char sql5[1000];

    // Menu:
    int contador;
    GrupoPreguntas grupoTest;
    char opcion;
    char eliminar[100];
    char hacer[100];
    char respuesta[100];
    do
    {
        opcion = menuPrincipal();
        switch (opcion)
        {
        case '1':
            grupoTest = reservarMemoria();
            contador = 0;
            while (contador < grupoTest.tam)
            {
                addPregunta(&grupoTest);
                contador++;
            }

            mostarPreguntas(grupoTest);

            const char *nombreEncuesta = grupoTest.nombreEncuesta;
            crearEncuesta(grupoTest, nombreEncuesta);
            fprintf(archivo, "[%d:%d:%d] Test creado. \n",tm.tm_hour,tm.tm_min,tm.tm_sec);
            break;

        case '2':
            existe = sqlite3_exec(DB, sql4, callback, 0, &errMsg);

            if (existe != SQLITE_OK)
            {
                fprintf(stderr, "[%d:%d:%d] Error en la consulta SQL: %s\n", tm.tm_hour,tm.tm_min,tm.tm_sec, errMsg);
                sqlite3_free(errMsg);
            }
            printf("Nombre del test a hacer: ");

            fflush(stdout);
            fflush(stdin);

            scanf("%s", hacer);
            sprintf(sql1, "SELECT pregunta FROM pregunta WHERE nombre LIKE '%s';", hacer);

            existe = sqlite3_exec(DB, sql1, callback, 0, &errMsg);

            if (existe != SQLITE_OK)
            {
                fprintf(stderr, "[%d:%d:%d] Error en la consulta SQL: %s\n",tm.tm_hour,tm.tm_min,tm.tm_sec, errMsg);
                sqlite3_free(errMsg);
            }
            printf("Responda la pregunta: ");

            fflush(stdout);
            fflush(stdin);

            scanf("%s", respuesta);

            sprintf(sql5, "SELECT respuesta FROM pregunta WHERE nombre LIKE '%s';", hacer);

            existe = sqlite3_exec(DB, sql5, callback, 0, &errMsg);

            if (existe != SQLITE_OK)
            {
                fprintf(stderr, "[%d:%d:%d] Error en la consulta SQL: %s\n",tm.tm_hour,tm.tm_min,tm.tm_sec, errMsg);
                sqlite3_free(errMsg);
            }

            fprintf(archivo, "[%d:%d:%d] Test completado. \n", tm.tm_hour,tm.tm_min,tm.tm_sec);
            break;

        case '3':

            existe = sqlite3_exec(DB, sql2, callback, 0, &errMsg);

            if (existe != SQLITE_OK)
            {
                fprintf(stderr, "[%d:%d:%d] Error en la consulta SQL: %s\n",tm.tm_hour,tm.tm_min,tm.tm_sec, errMsg);
                sqlite3_free(errMsg);
            }
            fprintf(archivo, "[%d:%d:%d] Teses visualizadas. \n",tm.tm_hour,tm.tm_min,tm.tm_sec);
            break;

        case '4':
            existe = sqlite3_exec(DB, sql4, callback, 0, &errMsg);

            if (existe != SQLITE_OK)
            {
                fprintf(stderr, "[%d:%d:%d] Error en la consulta SQL: %s\n",tm.tm_hour,tm.tm_min,tm.tm_sec, errMsg);
                sqlite3_free(errMsg);
            }
            printf("Nombre del test a eliminar: ");

            fflush(stdout);
            fflush(stdin);

            scanf("%s", eliminar);
            sprintf(sql3, "DELETE FROM pregunta WHERE nombre = '%s';", eliminar);

            existe = sqlite3_exec(DB, sql3, 0, 0, NULL);
            if (existe != SQLITE_OK)
            {
                fprintf(stderr,"Error al ejecutar la consulta SQL: %s\n",tm.tm_hour,tm.tm_min,tm.tm_sec, errMsg);
                sqlite3_free(errMsg);
            }

            fprintf(archivo, "[%d:%d:%d] Test eliminado correctamente. \n",tm.tm_hour,tm.tm_min,tm.tm_sec);
            break;

        case '0':
            printf("Fin del programa\n");
            fprintf(archivo, "[%d:%d:%d] Programa finalizado. \n",tm.tm_hour,tm.tm_min,tm.tm_sec);
            break;

        default:
            printf("ERROR, introduce de nuevo\n");
            fprintf(archivo, "[%d:%d:%d] Error al introducir la orden. \n",tm.tm_hour,tm.tm_min,tm.tm_sec);
            fflush(stdout);
        }
    } while (opcion != '0');

    sqlite3_close(DB);
    fprintf(archivo, "[%d:%d:%d] Base de datos cerrada. \n",tm.tm_hour,tm.tm_min,tm.tm_sec);
    fclose(archivo);
}