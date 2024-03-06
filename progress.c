#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "progress.h"

void initCommandListDyn(commandListDyn *array, size_t initialCapacity) {
    array->strings = (char **)malloc(initialCapacity * sizeof(char *));
    array->capacity = initialCapacity;
    array->size = 0;
}

void addString(commandListDyn *array, char *str) {
    if (array->size == array->capacity) {
        size_t newCapacity = array->capacity * 2;
        array->strings = (char **)realloc(array->strings, newCapacity * sizeof(char *));
        array->capacity = newCapacity;
    }

    // Copiar el nuevo string al array
    array->strings[array->size] = strdup(str); // strdup copia el string a una nueva ubicación de memoria
    array->size++;
}

void freeDynamicStringArray(commandListDyn *array) {
    for (size_t i = 0; i < array->size; i++) {
        free(array->strings[i]); // Libera cada string
    }
    free(array->strings); // Libera el array de punteros
}

void saveProgress(char **cachedProgress, int size, char *nomfich){
    FILE *pf;
    int i;
    pf = fopen(nomfich,"w");
    if(pf != (FILE*)NULL){
        for(i = 0; i < size; i++){
            fprintf(pf,"%s\n",cachedProgress[i]);
        }
        fclose(pf);
    }
}




