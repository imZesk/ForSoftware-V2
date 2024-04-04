#include <stdio.h>
#include <sqlite3.h>

int main(){

    sqlite3 *DB;
    char *errMsg = 0;

    //Abrimos la bd
    int existe = sqlite3_open("./Preguntas.db", &DB);

    //Confirmamos que se abre correctamente
    if (existe != SQLITE_OK) {
        printf("Error");
        //logger con el error
        return 1;
    }

    //avisamos por consola que la base de datos se ha abierto correctamente
    printf("Conexión exitosa a la base de datos");

    // Ejecuta la consulta
    char *sql = "SELECT pregunta, respuesta FROM pregunta;";

    rc = sqlite3_exec(db, sql, callback, 0, &errMsg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error en la consulta SQL: %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    //cerrar la base de datos
    sqlite3_close(DB);
    return 0;


    //Menu:
    char opcion;
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
}