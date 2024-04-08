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

    char *sql1 = "SELECT tipo_pregunta, pregunta, opciones, respuesta FROM pregunta where x = %s;"; 
    //sustituir la x por el parametro que pase el usuario y la "" por lo que pase el usuario
    char *sql2 = "SELECT tipo_pregunta, pregunta, opciones, respuesta FROM pregunta;";
    char *sql3 = "DELETE FROM pregunta WHERE x = %s;"; 
    //sustituir la x por el parametro que pase el usuario y la "" por lo que pase el usuario

    // // Ejecuta la consulta
    // char *sql = "SELECT tipo_pregunta, pregunta, opciones, respuesta FROM pregunta;";

    // existe = sqlite3_exec(DB, sql, callback, 0, &errMsg);
    
    // if (existe != SQLITE_OK) {
    //     fprintf(stderr, "Error en la consulta SQL: %s\n", errMsg);
    //     sqlite3_free(errMsg);
    // }


    // //Abrimos la bd
    // int existe = sqlite3_open("./lib/Preguntas.db", &DB);

    // //Confirmamos que se abre correctamente
    // if (existe != SQLITE_OK) {
    //     printf("Error");
    //     //logger con el error
    //     return 1;
    // }

    // //avisamos por consola que la base de datos se ha abierto correctamente
    // printf("Conexión exitosa a la base de datos");

    // // Ejecuta la consulta
    // char *sql = "SELECT pregunta, respuesta FROM pregunta;";

    // existe = sqlite3_exec(DB, sql, callback, 0, &errMsg);
    
    // if (existe != SQLITE_OK) {
    //     fprintf(stderr, "Error en la consulta SQL: %s\n", errMsg);
    //     sqlite3_free(errMsg);
    // }

    //Insertar em la DB
    // Crear la sentencia SQL para insertar la pregunta y la respuesta
    //char sentencia[200];
    //sprintf(sentencia, "INSERT INTO pregunta (tipo_pregunta, pregunta, opciones, respuesta) VALUES ('%s', '%s', '%s', '%s')", tipo_pregunta, pregunta, opciones, respuesta);

    // Ejecutar la sentencia SQL
    //existe = sqlite3_exec(conexion, sentencia, 0, 0, NULL);
    //if (existe != SQLITE_OK) {
    //    printf("Error al ejecutar la sentencia SQL: %s\n", sqlite3_errmsg(DB));
    //    return 1;
    //}

    // //cerrar la base de datos
    // sqlite3_close(DB);
    // return 0;


    //Menu:
    int contador;
    GrupoPreguntas grupoTest;
    char opcion;
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
    
                        if (existe != SQLITE_OK) {
                            fprintf(stderr, "Error en la consulta SQL: %s\n", errMsg);
                            sqlite3_free(errMsg);
                        }
                        fprintf(archivo, "Notas visualizadas. \n");
				break;
            
            case '4':

                        existe = sqlite3_exec(DB, sql3, 0, 0, NULL);
                        if (existe != SQLITE_OK) {
                            printf("Error al ejecutar la consulta SQL: %s\n", errMsg);
                            sqlite3_free(errMsg);
                            return 1;
                        }
                        printf("prueba\n");
                        fprintf(archivo, "Test eliminado correctamente. \n");
				break;

			case '0':   printf("Fin del programa\n");
                        fprintf(archivo, "Programa finalizado. \n");
			    break;

			default:    printf("ERROR, introduce de nuevo\n");
                        fprintf(archivo, "Error al introducir la orden. \n");
                        fflush(stdout);
		}
	}while(opcion!='0');

    fclose(archivo);
    sqlite3_close(DB);

}