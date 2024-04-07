#include "grupoPreguntas.h"
#include "pregunta.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Sqlite3/sqlite3.h"

GrupoPreguntas reservarMemoria(){
    GrupoPreguntas grupo;
    int tam;

    printf("Ingrese la cantidad de preguntas que va a tener el test: ");
    scanf("%d", &tam);

    grupo.arrPreguntas = (Pregunta *)malloc(tam * sizeof(Pregunta));
    grupo.tam = tam;
    grupo.numPreguntas = 0;
    return grupo;
}
void addPregunta(GrupoPreguntas * grupo){

    Pregunta pregunta;

    char preguntaText[100];
    char tipoPreguntaText[100];

    printf("Ingrese la pregunta: ");
    fflush(stdout);
    fflush(stdin);
    fgets(preguntaText, sizeof(preguntaText), stdin);

    printf("Ingrese el tipo de pregunta: ");
    fflush(stdout);
    fgets(tipoPreguntaText, sizeof(tipoPreguntaText), stdin);

    preguntaText[strcspn(preguntaText, "\n")] = '\0';
    tipoPreguntaText[strcspn(tipoPreguntaText, "\n")] = '\0';

    pregunta.pregunta = strdup(preguntaText);
    pregunta.tipoPregunta = strdup(tipoPreguntaText);

    FILE *archivo; // Declara un puntero a FILE
    archivo = fopen("./lib/Log.txt", "a"); // Abre el archivo en modo append ("a")

    sqlite3 *DB;
    char *errMsg = 0;

    int existe = sqlite3_open("./lib/Preguntas.db", &DB);

    if (existe != SQLITE_OK) {
         printf("Error");
         //logger con el error
         return 1;
    }

    if (archivo == NULL) { // Verifica si ocurrió algún error al abrir el archivo
        printf("Error al abrir el archivo.\n");
        return;
    }

    if(grupo->numPreguntas < grupo->tam){
        grupo->arrPreguntas[grupo->numPreguntas] = pregunta;
        grupo->numPreguntas++;
        printf("Pregunta anadida correctamente\n");
        fprintf(archivo, "Pregunta añadida correctamente.\n"); // Escribe en el archivo

        char sentencia[200];
        sprintf(sentencia, "INSERT INTO pregunta (tipo_pregunta, pregunta, opciones, respuesta) VALUES ('%s', '%s', '%s', '%s')", tipo_pregunta, pregunta, opciones, respuesta);
        //despues de la coma es donde hay que poner los parametros en el orden que he puesto

        // Ejecutar la sentencia SQL
        existe = sqlite3_exec(DB, sentencia, 0, 0, NULL);
        if (existe != SQLITE_OK) {
            printf("Error al ejecutar la sentencia SQL: %s\n", sqlite3_errmsg(DB));
            return 1;
        }

    }else{
        printf("No se pueden añadir más preguntas\n");
        fprintf(archivo, "Error al añadir la pregunta.\n"); // Escribe en el archivo
    }

    sqlite3_close(DB);
}

void mostarPreguntas(GrupoPreguntas grupo){
    for(int i = 0; i < grupo.numPreguntas; i++){
        printf("Pregunta %d: %s\n", i+1, grupo.arrPreguntas[i].pregunta);
    }
}

