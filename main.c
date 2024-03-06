#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "textExecution.h"
#include "progress.h"

char input[100] = "\n";

char *fichero = "progress.txt";

commandListDyn progressA;

static bool getInput(void)
{
    printf("\n--> ");
    fgets(input, sizeof input, stdin) != NULL;

    if (strcmp(input, "salir\n") != 0) {
        addString(&progressA, input);
    }
}

    void loadProgress(char *filename) {
        printf("Buscando progreso...\n");
        FILE *file = fopen(filename, "r");
        if (file != NULL) {
            printf("Progreso encontrado!\n");
            char line[256];
            while (fgets(line, sizeof(line), file)) {
                line[strcspn(line, "\n")] = '\0'; 
                resolveThenExecute(line);
                addString(&progressA, line);
            }
            fclose(file);
            printf("Continua tu partida!\n");
        }
    }


int main()
{
    initCommandListDyn(&progressA, 2);

    printf("Bienvenido a Escape From Deusto!\n");
    printf("Escribe 'salir' para terminar el juego.\n");
    printf("=========================================\n");

    loadProgress(fichero);

    while (resolveThenExecute(input) && getInput());
   

   
    printf("\nAdios!\n");
    printf("=========================================\n");
    printf("Guardando progreso...\n");
    saveProgress(progressA.strings, progressA.size, "progress.txt");
    printf("Progreso guardado!\n");
    printf("=========================================\n");
    return 0;
}