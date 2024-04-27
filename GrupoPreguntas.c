#include "grupoPreguntas.h"
#include "pregunta.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Sqlite3/sqlite3.h"
#include <time.h>

GrupoPreguntas reservarMemoria()
{
    GrupoPreguntas grupo;
    int tam;

    printf("Ingrese la cantidad de preguntas que va a tener el test: ");
    scanf("%d", &tam);

    grupo.arrPreguntas = (Pregunta *)malloc(tam * sizeof(Pregunta));
    grupo.tam = tam;
    grupo.numPreguntas = 0;
    return grupo;
}
void addPregunta(GrupoPreguntas *grupo)
{
    time_t hora = time(NULL);
    struct tm tm = *localtime(&hora);
    FILE *archivo;                         // Declara un puntero a FILE
    archivo = fopen("./lib/Log.txt", "a"); // Abre el archivo en modo append ("a")

    if (archivo == NULL)
    { // Verifica si ocurrió algún error al abrir el archivo
        printf("Error al abrir el archivo.\n");
        return;
    }

    Pregunta pregunta;

    char preguntaText[100];
    char tipoPreguntaText[100];
    char respuestaText[100];
    char nombreEncuesta[100];
    
    printf("Introduce el nombre de la encuesta: ");
    scanf("%s", nombreEncuesta);

    fprintf(archivo, "[%d:%d:%d] Nombre de la encuesta: %s. \n",tm.tm_hour,tm.tm_min,tm.tm_sec, nombreEncuesta);

    printf("Ingrese la pregunta: ");
    fflush(stdout);
    fflush(stdin);
    fgets(preguntaText, sizeof(preguntaText), stdin);

    fprintf(archivo, "[%d:%d:%d] Pregunta ingresada: %s. \n",tm.tm_hour,tm.tm_min,tm.tm_sec, preguntaText);

    /*printf("Ingrese el tipo de pregunta: ");
    fflush(stdout);
    fgets(tipoPreguntaText, sizeof(tipoPreguntaText), stdin);
    fflush(stdin);*/
    
    printf("Ingrese la respuesta a la pregunta: ");
    fflush(stdout);
    fgets(respuestaText, sizeof(respuestaText), stdin);
    fflush(stdin);


    fprintf(archivo, "[%d:%d:%d] Tipo de la pregunta ingresada: %s. \n",tm.tm_hour,tm.tm_min,tm.tm_sec, tipoPreguntaText);
    
    nombreEncuesta[strcspn(nombreEncuesta, "\n")] = '\0';
    preguntaText[strcspn(preguntaText, "\n")] = '\0';
    tipoPreguntaText[strcspn(tipoPreguntaText, "\n")] = '\0';
    respuestaText[strcspn(respuestaText, "\n")] = '\0';

    pregunta.pregunta = strdup(preguntaText);
    pregunta.tipoPregunta = strdup(tipoPreguntaText);
    pregunta.respuesta = strdup(respuestaText);

    sqlite3 *DB;
    char *errMsg = 0;

    int existe = sqlite3_open("./lib/Preguntas.db", &DB);
    fprintf(archivo, "[%d:%d:%d] Base de datos abierta. \n",tm.tm_hour,tm.tm_min,tm.tm_sec);

    if (existe != SQLITE_OK)
    {
        printf("Error");
        // logger con el error
        fprintf(archivo, "[%d:%d:%d] Error al abrir la base de datos. \n",tm.tm_hour,tm.tm_min,tm.tm_sec);
    }

    if (grupo->numPreguntas < grupo->tam)
    {
        grupo->arrPreguntas[grupo->numPreguntas] = pregunta;
        grupo->numPreguntas++;
        printf("Pregunta anadida correctamente\n");
        fprintf(archivo, "[%d:%d:%d] Pregunta añadida correctamente.\n", tm.tm_hour,tm.tm_min,tm.tm_sec); // Escribe en el archivo

        char sentencia[200];

        sprintf(sentencia, "INSERT INTO pregunta (nombre, tipo_pregunta, pregunta, respuesta) VALUES ('%s', '%s', '%s', '%s')", nombreEncuesta, NULL, preguntaText, respuestaText);
        // Ejecutar la sentencia SQL
        existe = sqlite3_exec(DB, sentencia, 0, 0, NULL);
        if (existe != SQLITE_OK)
        {
            printf("Error al ejecutar la sentencia SQL: %s\n", sqlite3_errmsg(DB));
            fprintf(archivo, "[%d:%d:%d] Error al ejecutar la sentencia SQL. \n",tm.tm_hour,tm.tm_min,tm.tm_sec);
        }
    }
    else
    {
        printf("No se pueden añadir más preguntas\n");
        fprintf(archivo, "[%d:%d:%d] Error al añadir la pregunta.\n",tm.tm_hour,tm.tm_min,tm.tm_sec);
    }

    sqlite3_close(DB);
    fprintf(archivo, "[%d:%d:%d] Base de datos cerrada.\n",tm.tm_hour,tm.tm_min,tm.tm_sec);
}

void mostarPreguntas(GrupoPreguntas grupo)
{
    time_t hora = time(NULL);
    struct tm tm = *localtime(&hora);
    FILE *archivo;                         // Declara un puntero a FILE
    archivo = fopen("./lib/Log.txt", "a"); // Abre el archivo en modo append ("a")

    if (archivo == NULL)
    { // Verifica si ocurrió algún error al abrir el archivo
        printf("Error al abrir el archivo.\n");
        return;
    }

    for (int i = 0; i < grupo.numPreguntas; i++)
    {
        printf("Pregunta %d: %s\n", i + 1, grupo.arrPreguntas[i].pregunta);
        fprintf(archivo, "[%d:%d:%d] Preguntas mostradas.\n",tm.tm_hour,tm.tm_min,tm.tm_sec);
    }
}
