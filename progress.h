#ifndef PROGRESS_H_
#define PROGRESS_H_


typedef struct {
    char **strings; // Puntero a un array de punteros a char
    size_t capacity; // Capacidad actual del array
    size_t size; // Número actual de elementos en el array
} commandListDyn;

void initCommandListDyn(commandListDyn *array, size_t initialCapacity);
void addString(commandListDyn *array, char *str);
void freeDynamicStringArray(commandListDyn *array);

void saveProgress(char **cachedProgress, int size, char *nomfich);
#endif