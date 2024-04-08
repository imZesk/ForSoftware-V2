#include <stdio.h>
#include "menus.h"
#include "Sqlite3/sqlite3.h"
#include "GrupoPreguntas.h"
#include "encuesta.h"


static int callback(void *data, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}


int main(){

    FILE *archivo; // Declara un puntero a FILE
    archivo = fopen("./lib/Log.txt", "w"); // Abre el archivo en modo escritura ("w")

    if (archivo == NULL) { // Verifica si ocurrió algún error al abrir el archivo
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    fprintf(archivo, "Archivo creado. \n");

    sqlite3 *DB;
    char *errMsg = 0;


    //Abrimos la bd
    
    int existe = sqlite3_open("./lib/Preguntas.db", &DB);
    fprintf(archivo, "Base de datos abierta. \n");
    // //Abrimos la bd
    // int existe = sqlite3_open("./lib/Preguntas.db", &DB);

    // //Confirmamos que se abre correctamente
    if (existe != SQLITE_OK) {
         printf("Error");
         //logger con el error
         fprintf(archivo, "Error al abrir la base de datos. \n");
         return 1;
    }
    char *sql1 = "SELECT nombre, tipo_pregunta, pregunta, respuesta FROM pregunta where nombre = %s;"; 
    //sustituir la x por el parametro que pase el usuario y la "" por lo que pase el usuario
    char *sql2 = "SELECT nombre, tipo_pregunta, pregunta, respuesta FROM pregunta;";
     char sql3[1000]; //= "DELETE FROM pregunta WHERE nombre = %s;"; 
    //sustituir la x por el parametro que pase el usuario y la "" por lo que pase el usuario
    char *sql4 = "SELECT nombre FROM pregunta;";

    //Menu:
    int contador;
    GrupoPreguntas grupoTest;
    char opcion;
    char eliminar[100];
    	do{
		opcion = menuPrincipal();
		switch(opcion){
            case '1':
                        grupoTest = reservarMemoria();
                        contador = 0;
                        while (contador < grupoTest.tam) {
                            addPregunta(&grupoTest);
                            contador++;
                            }
                        
                        mostarPreguntas(grupoTest);
                        crearEncuesta(grupoTest);
                        fprintf(archivo, "Test creado. \n");
                break;

			case '2':  
                        
                        existe = sqlite3_exec(DB, sql1, callback, 0, &errMsg);
    
                        if (existe != SQLITE_OK) {
                            fprintf(stderr, "Error en la consulta SQL: %s\n", errMsg);
                            sqlite3_free(errMsg);
                        }
                        fprintf(archivo, "Test completado. \n");
				break;

			case '3':   
                        

                        existe = sqlite3_exec(DB, sql2, callback, 0, &errMsg);

            if (existe != SQLITE_OK)
            {
                fprintf(stderr, "Error en la consulta SQL: %s\n", errMsg);
                sqlite3_free(errMsg);
            }
            fprintf(archivo, "Teses visualizadas. \n");
            break;

        case '4':
            existe = sqlite3_exec(DB, sql4, callback, 0, &errMsg);

                if (existe != SQLITE_OK) {
                fprintf(stderr, "Error en la consulta SQL: %s\n", errMsg);
                sqlite3_free(errMsg);
            }
	        printf("Nombre del test a eliminar: ");

	        fflush(stdout);
	        fflush(stdin);

            scanf("%s", eliminar);
            printf("prueba\n");
            sprintf(sql3, "DELETE FROM pregunta WHERE nombre = '%s';", eliminar);
            printf("prueba2\n");

            existe = sqlite3_exec(DB, sql3, 0, 0, NULL);
            if (existe != SQLITE_OK)
            {
                printf("Error al ejecutar la consulta SQL: %s\n", errMsg);
                sqlite3_free(errMsg);
            }

            printf("prueba3\n");
            fprintf(archivo, "Test eliminado correctamente. \n");
            break;

        case '0':
            printf("Fin del programa\n");
            fprintf(archivo, "Programa finalizado. \n");
            break;

        default:
            printf("ERROR, introduce de nuevo\n");
            fprintf(archivo, "Error al introducir la orden. \n");
            fflush(stdout);
        }
    } while (opcion != '0');

    sqlite3_close(DB);
    fprintf(archivo, "Base de datos cerrada. \n");
    fclose(archivo);

}