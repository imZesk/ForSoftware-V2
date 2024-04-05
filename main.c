#include <stdio.h>
#include "Sqlite3/sqlite3.h"
#include "GrupoPreguntas.h"


static int callback(void *data, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}


int main(){

    // sqlite3 *DB;
    // char *errMsg = 0;


    printf("Este es el main");

    //Abrimos la bd
    
    //int existe = sqlite3_open("./lib/Preguntas.db", &DB);
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
    //sprintf(sentencia, "INSERT INTO pregunta (pregunta, respuesta) VALUES ('%s', '%s')", pregunta, respuesta);

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
/*    char opcion;
    	do{
		opcion = menuPrincipal();
		switch(opcion){
			case '1': //crearTest();
				      break;

			case '2': printf("La potencia media es: %.2f\n", obtenerPotenciaMedia(lt));
					  break;

			case '3': //visualizarNota();
				      break;
            
            case '4': //t = pedirTest();
			          //eliminarTestAFichero(t);
				      break;

			case '0': printf("Fin del programa\n");
			          break;

			default: printf("ERROR, introduce de nuevo\n");
		}
	}while(opcion!='0');
    */
    int tam = 3;
    GrupoPreguntas grupoTest = reservarMemoria(tam);

    addPregunta(&grupoTest);
    mostarPreguntas(grupoTest);
    
    return 0;

}