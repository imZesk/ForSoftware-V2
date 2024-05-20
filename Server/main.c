#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include <time.h>

#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

#define MAX_BUFFER_SIZE 1024

// Función para dividir una cadena en subcadenas utilizando un delimitador
char** split(const char* str, const char* delim, int* num_tokens) {
    char* s = strdup(str);
    int count = 0;
    char* token;
    char* rest = s;

    // Contar el número de tokens
    token = strtok(rest, delim);
    while (token != NULL) {
        count++;
        token = strtok(NULL, delim);
    }

    free(s);

    char** result = malloc(count * sizeof(char*));
    s = strdup(str);
    rest = s;
    int i = 0;

    // Almacenar los tokens en el resultado
    token = strtok(rest, delim);
    while (token != NULL) {
        result[i++] = strdup(token);
        token = strtok(NULL, delim);
    }

    free(s);
    *num_tokens = count;
    return result;
}

void crear_test_y_preguntas(sqlite3* DB, const char* recvBuff) {
    char* errMsg = 0;

    // Parsear los datos recibidos
    int num_parts;
    char** parts = split(recvBuff, ";", &num_parts);

    if (num_parts < 2) {
        printf("Datos insuficientes para crear el test y preguntas.\n");
        return;
    }

    // Extraer la información del test
    int num_test_info_parts;
    char** test_info = split(parts[0], ",", &num_test_info_parts);
    if (num_test_info_parts != 2) {
        printf("Formato incorrecto para los datos del test.\n");
        return;
    }

    const char* test_name = test_info[0];
    int test_preg_count = atoi(test_info[1]);

    // Insertar el test en la tabla 'test'
    char sql[512];
    sprintf(sql, "INSERT INTO test (nombre, cant_preg) VALUES ('%s', %d);", test_name, test_preg_count);
    if (sqlite3_exec(DB, sql, 0, 0, &errMsg) != SQLITE_OK) {
        printf("Error al insertar el test: %s\n", errMsg);
        sqlite3_free(errMsg);
        return;
    }

    // Obtener el ID del test recién insertado
    int test_id = sqlite3_last_insert_rowid(DB);

    // Insertar las preguntas y vincularlas con el test
    for (int i = 1; i < num_parts; i++) {
        int num_question_parts;
        char** question_parts = split(parts[i], ",", &num_question_parts);

        if (num_question_parts != 4) {
            printf("Formato incorrecto para los datos de la pregunta.\n");
            continue;
        }

        int tipo_preg = atoi(question_parts[0]);
        const char* pregunta = question_parts[1];
        const char* opciones = strcmp(question_parts[2], "NULL") == 0 ? NULL : question_parts[2];
        const char* respuesta = question_parts[3];

        // Insertar la pregunta en la tabla 'pregunta'
        if (opciones == NULL) {
            sprintf(sql, "INSERT INTO pregunta (tipo_preg, pregunta, opciones, respuesta) VALUES (%d, '%s', NULL, '%s');", tipo_preg, pregunta, respuesta);
        } else {
            sprintf(sql, "INSERT INTO pregunta (tipo_preg, pregunta, opciones, respuesta) VALUES (%d, '%s', '%s', '%s');", tipo_preg, pregunta, opciones, respuesta);
        }

        if (sqlite3_exec(DB, sql, 0, 0, &errMsg) != SQLITE_OK) {
            printf("Error al insertar la pregunta: %s\n", errMsg);
            sqlite3_free(errMsg);
            continue;
        }

        // Obtener el ID de la pregunta recién insertada
        int pregunta_id = sqlite3_last_insert_rowid(DB);

        // Vincular la pregunta con el test en la tabla 'tiene'
        sprintf(sql, "INSERT INTO tiene (id_t, id_p) VALUES (%d, %d);", test_id, pregunta_id);
        if (sqlite3_exec(DB, sql, 0, 0, &errMsg) != SQLITE_OK) {
            printf("Error al vincular la pregunta con el test: %s\n", errMsg);
            sqlite3_free(errMsg);
        }

        // Liberar memoria
        for (int j = 0; j < num_question_parts; j++) {
            free(question_parts[j]);
        }
        free(question_parts);
    }

    // Liberar memoria
    for (int i = 0; i < num_parts; i++) {
        free(parts[i]);
    }
    free(parts);

    // Liberar memoria para los datos del test
    for (int i = 0; i < num_test_info_parts; i++) {
        free(test_info[i]);
    }
    free(test_info);

    printf("Test y preguntas creadas exitosamente.\n");
}


char *visualizar_tests(sqlite3 *db)
{
    sqlite3_stmt *stmt;

    char sql[] = "SELECT id_t, nombre, cant_preg FROM test";

    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (result != SQLITE_OK)
    {
        printf("Error preparing statement (SELECT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return "Error preparando la declaracion\n";
    }

    printf("SQL query prepared (SELECT)\n");

    char *teses = (char *)malloc(MAX_BUFFER_SIZE);
    if (teses == NULL)
    {
        printf("Error: Unable to allocate memory for teses\n");
        return NULL;
    }
    memset(teses, 0, MAX_BUFFER_SIZE);

    char nombre[100];
    int cant_preg;
    int id;
    int first_row = 1;

    printf("\n");
    printf("\n");
    printf("Mostrando tests:\n");
    do
    {
        result = sqlite3_step(stmt);
        if (result == SQLITE_ROW)
        {
            id = sqlite3_column_int(stmt, 0);
            strcpy(nombre, (char *)sqlite3_column_text(stmt, 1));
            cant_preg = sqlite3_column_int(stmt, 2);
            if (first_row)
            {
                sprintf(teses, "%d,%s,%d", id, nombre, cant_preg);
                first_row = 0;
            }
            else
            {
                sprintf(teses, "%s;%d,%s,%d", teses, id, nombre, cant_preg);
            }
        }
    } while (result == SQLITE_ROW);

    printf("Teses: %s\n", teses);

    result = sqlite3_finalize(stmt);
    if (result != SQLITE_OK)
    {
        printf("Error finalizing statement (SELECT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        free(teses);
        return NULL;
    }

    printf("Prepared statement finalized (SELECT)\n");

    return teses;
}

char *resultado_teses(sqlite3 *db)
{
    sqlite3_stmt *stmt;

    char sql[] = "SELECT test.nombre,test.id_t,(SELECT resultado.pregs_realiz FROM resultado WHERE resultado.id_t = test.id_t) AS pregs_realiz,(SELECT resultado.respuestas_c FROM resultado WHERE resultado.id_t = test.id_t) AS respuestas_c FROM test;";

    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (result != SQLITE_OK)
    {
        printf("Error preparing statement (SELECT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return "Error preparando la declaracion\n";
    }

    printf("SQL query prepared (SELECT)\n");

    char *resultado = (char *)malloc(MAX_BUFFER_SIZE);
    if (resultado == NULL)
    {
        printf("Error: Unable to allocate memory for teses\n");
        return NULL;
    }
    memset(resultado, 0, MAX_BUFFER_SIZE);

    char nombre[100];
    int preg_reali;
    int respuestas_correct;
    int id_t;
    int first_row = 1;

    printf("\n");
    do
    {
        result = sqlite3_step(stmt);
        if (result == SQLITE_ROW)
        {
            strcpy(nombre, (char *)sqlite3_column_text(stmt, 0));
            id_t = sqlite3_column_int(stmt, 1);
            preg_reali = sqlite3_column_int(stmt, 2);
            respuestas_correct = sqlite3_column_int(stmt, 3);
            if (first_row)
            {
                sprintf(resultado, "%s,%d,%d,%d", nombre,id_t, preg_reali, respuestas_correct);
                first_row = 0;
            }
            else
            {
                sprintf(resultado, "%s;%s,%d,%d,%d", resultado, nombre, id_t, preg_reali, respuestas_correct);
            }
        }
    } while (result == SQLITE_ROW);

    result = sqlite3_finalize(stmt);
    if (result != SQLITE_OK)
    {
        printf("Error finalizing statement (SELECT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        free(resultado);
        return NULL;
    }

    printf("Prepared statement finalized (SELECT)\n");

    return resultado;
}

void eliminar_test(char *eliminar, sqlite3 *DB, char *errMsg)
{
    char sql[100];
    sprintf(sql, "DELETE FROM test WHERE nombre = '%s';", eliminar);

    int rc = sqlite3_exec(DB, sql, NULL, 0, &errMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error ejecutando la sentencia SQL: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    else
    {
        printf("Test eliminado correctamente.\n");
    }
}

void crearPregunta(sqlite3 *DB, char *errMsg, char *tipo, char *pregunta, char *opciones, char *respuesta)
{
    char sql[512];
    if (strcmp(opciones, "NULL")==0)
    {
        sprintf(sql, "INSERT INTO pregunta (tipo_preg, pregunta, opciones, respuesta) VALUES ('%s', '%s', '%s', '%s');", tipo, pregunta, NULL, respuesta);
    }
    else
    {
        sprintf(sql, "INSERT INTO pregunta (tipo_preg, pregunta, opciones, respuesta) VALUES ('%s', '%s', '%s', '%s');", tipo, pregunta, opciones, respuesta);
    }
    int existe = sqlite3_exec(DB, sql, NULL, 0, &errMsg);
    if (existe != SQLITE_OK)
    {
        printf("Error en la consulta SQL: %s\n", errMsg);
    }
    else
    {
        printf("Pregunta creada exitosamente.\n");
    }
}

void anadirPregunta(sqlite3 *DB, char *errMsg, char *test, char *tipo, char *pregunta, char *opciones, char *respuesta)
{
    char sql[512];
    sprintf(sql, "SELECT id_t, cant_preg FROM test WHERE nombre = '%s'", test);
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(DB, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Error en la preparación de la consulta SQL para obtener id_t y cant_preg: %s\n", sqlite3_errmsg(DB));
        return;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW)
    {
        printf("No se encontró ningún test con el nombre proporcionado: %s\n", test);
        sqlite3_finalize(stmt);
        return;
    }

    int id_t = sqlite3_column_int(stmt, 0);
    int cant_preg = sqlite3_column_int(stmt, 1);
    sqlite3_finalize(stmt);

    // Paso 2: Incrementar la cantidad de preguntas del test
    cant_preg++;

    // Actualizar la cantidad de preguntas en la base de datos
    sprintf(sql, "UPDATE test SET cant_preg = %d WHERE id_t = %d", cant_preg, id_t);
    rc = sqlite3_exec(DB, sql, NULL, 0, &errMsg);
    if (rc != SQLITE_OK)
    {
        printf("Error al actualizar la cantidad de preguntas del test: %s\n", errMsg);
        return;
    }

    // Paso 3: Llamar a la función crearPregunta para agregar la pregunta
    crearPregunta(DB, errMsg, tipo, pregunta, opciones, respuesta);
    if (errMsg != NULL)
    {
        printf("Error al crear la pregunta: %s\n", errMsg);
        return;
    }

    // Paso 4: Obtener el id_p de la pregunta recién creada
    int id_p = sqlite3_last_insert_rowid(DB);

    // Paso 5: Añadir una entrada en la tabla tiene para vincular la pregunta con el test
    sprintf(sql, "INSERT INTO tiene (id_t, id_p) VALUES (%d, %d)", id_t, id_p);
    rc = sqlite3_exec(DB, sql, NULL, 0, &errMsg);
    if (rc != SQLITE_OK)
    {
        printf("Error al añadir la pregunta al test en la tabla 'tiene': %s\n", errMsg);
        return;
    }

    printf("Pregunta añadida exitosamente al test %s.\n", test);
}

char *obtenerPregunta(sqlite3 *DB, char *errMsg, int id_p)
{
    char *pregunta = malloc(100 * sizeof(char)); // Asignar memoria para la pregunta
    if (pregunta == NULL)
    {
        printf("Error al asignar memoria para la pregunta\n");
        return NULL;
    }

    char sql[512];
    sprintf(sql, "SELECT pregunta FROM pregunta WHERE id_p = %d", id_p);
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(DB, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Error en la preparación de la consulta SQL para obtener la pregunta: %s\n", sqlite3_errmsg(DB));
        free(pregunta); // Liberar memoria en caso de error
        return NULL;
    }

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        const unsigned char *text = sqlite3_column_text(stmt, 0);
        strcpy(pregunta, (const char *)text);
    }
    else
    {
        strcpy(pregunta, "No existe la pregunta");
    }

    sqlite3_finalize(stmt);
    return pregunta;
}

int obtenerTipo(sqlite3 *DB, char *errMsg, int id_p, int *tipo)
{
    char sql[512];
    sprintf(sql, "SELECT tipo_preg FROM pregunta WHERE id_p = %d", id_p);
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(DB, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Error en la preparación de la consulta SQL para obtener el tipo: %s\n", sqlite3_errmsg(DB));
        return -1; // Retornar un código de error
    }

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        *tipo = sqlite3_column_int(stmt, 0); // Obtener el valor entero de la columna
    }
    else
    {
        printf("No existe el tipo para el id_p dado\n");
        sqlite3_finalize(stmt);
        return -1; // Retornar un código de error
    }

    sqlite3_finalize(stmt);
    return 0;
}

char *obtenerOpciones(sqlite3 *DB, char *errMsg, int id_p)
{
    char sql[512];
    sprintf(sql, "SELECT opciones FROM pregunta WHERE id_p = %d", id_p);
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(DB, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Error en la preparación de la consulta SQL para obtener las opciones: %s\n", sqlite3_errmsg(DB));
        return NULL;
    }

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        const unsigned char *text = sqlite3_column_text(stmt, 0);
        char *opciones = malloc((strlen((const char *)text) + 1) * sizeof(char));
        if (opciones == NULL)
        {
            printf("Error al asignar memoria para las opciones\n");
            sqlite3_finalize(stmt);
            return NULL;
        }
        strcpy(opciones, (const char *)text);
        sqlite3_finalize(stmt);
        return opciones;
    }
    else
    {
        printf("No se encontraron opciones para la pregunta con id: %d\n", id_p);
        sqlite3_finalize(stmt);
        return NULL;
    }
}

char *obtenerRespuesta(sqlite3 *DB, char *errMsg, int id_p)
{
    char *respuesta = malloc(100 * sizeof(char)); // Asignar memoria para la respuesta
    if (respuesta == NULL)
    {
        printf("Error al asignar memoria para la respuesta\n");
        return NULL;
    }

    char sql[512];
    sprintf(sql, "SELECT respuesta FROM pregunta WHERE id_p = %d", id_p);
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(DB, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Error en la preparación de la consulta SQL para obtener la respuesta: %s\n", sqlite3_errmsg(DB));
        free(respuesta); // Liberar memoria en caso de error
        return NULL;
    }

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        const unsigned char *text = sqlite3_column_text(stmt, 0);
        strcpy(respuesta, (const char *)text);
    }
    else
    {
        strcpy(respuesta, "No existe la respuesta");
    }

    sqlite3_finalize(stmt);
    return respuesta;
}

int *realizarTest(sqlite3 *DB, char *errMsg, char *nombre, int *num_ids)
{
    char sql[512];
    int id_t;
    sprintf(sql, "SELECT id_t FROM test WHERE nombre = '%s'", nombre);
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(DB, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Error en la preparación de la consulta SQL para obtener id_t: %s\n", sqlite3_errmsg(DB));
        *num_ids = 0;
        return NULL;
    }

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        id_t = sqlite3_column_int(stmt, 0);
    }
    else
    {
        printf("No se encontró ningún ID para el test con nombre %s\n", nombre);
        sqlite3_finalize(stmt);
        *num_ids = 0;
        return NULL;
    }

    sqlite3_finalize(stmt);

    char sql2[512];
    sprintf(sql2, "SELECT id_p FROM tiene WHERE id_t = %d", id_t);
    rc = sqlite3_prepare_v2(DB, sql2, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Error en la preparación de la consulta SQL para obtener las preguntas: %s\n", sqlite3_errmsg(DB));
        *num_ids = 0;
        return NULL;
    }

    int *ids = NULL;
    int count = 0;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int id_p = sqlite3_column_int(stmt, 0);
        ids = realloc(ids, (count + 1) * sizeof(int));
        if (ids == NULL)
        {
            printf("Error al asignar memoria para los IDs\n");
            sqlite3_finalize(stmt);
            *num_ids = 0;
            return NULL;
        }
        ids[count] = id_p;
        count++;
    }

    if (rc != SQLITE_DONE)
    {
        printf("Error al recuperar las preguntas: %s\n", sqlite3_errmsg(DB));
        free(ids);
        *num_ids = 0;
        ids = NULL;
    }

    sqlite3_finalize(stmt);
    *num_ids = count;
    return ids;
}

void escribir_con_hora(FILE *file, const char *mensaje)
{
    time_t hora = time(NULL);
    struct tm tm = *localtime(&hora);
    fprintf(file, "[%02d:%02d:%02d] %s\n", tm.tm_hour, tm.tm_min, tm.tm_sec, mensaje);
    fflush(file); // Asegura que se escriba al archivo inmediatamente
}

int guardarResultado(sqlite3 *db, char *errMsg, const char *nombre_test, int respuestas_correctas, int preguntas_realizadas) {
    int id_test;
    
    // Obtener el id_t basado en el nombre del test
    char sql[256];
    sprintf(sql, "SELECT id_t FROM test WHERE nombre = '%s';", nombre_test);
    
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        id_test = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
    } else {
        fprintf(stderr, "No se encontró el test con nombre: %s\n", nombre_test);
        sqlite3_finalize(stmt);
        return SQLITE_ERROR;
    }
    
    // Insertar el resultado en la tabla resultado
    sprintf(sql, "INSERT INTO resultado (id_t, respuestas_c, pregs_realiz) VALUES (%d, %d, %d);",
            id_test, respuestas_correctas, preguntas_realizadas);
    
    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    }
    
    return SQLITE_OK;
}

int main(int argc, char *argv[])
{

    // socket
    WSADATA wsaData;
    SOCKET conn_socket;
    SOCKET comm_socket;
    struct sockaddr_in server;
    struct sockaddr_in client;
    char sendBuff[1000], recvBuff[1000];
    FILE *file;
    file = fopen("../lib/LogServer.txt", "w");
    if (file == NULL)
    {
        perror("Error al abrir el fichero");
        return 1;
    }

    time_t hora = time(NULL);
    struct tm tm = *localtime(&hora);

    printf("\nInicializando Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Fallo. Codigo de error : %d", WSAGetLastError());
        escribir_con_hora(file, "[Servidor] Fallo al inicializar Winsock.\n");
        return -1;
    }

    printf("Inicializado.\n");
    escribir_con_hora(file, "[Servidor] Winsock inicializado.\n");

    // SOCKET creation
    if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("No se ha podido crear el socket : %d", WSAGetLastError());
        escribir_con_hora(file, "[Servidor] Fallo en la creacion del socket.\n");
        WSACleanup();
        return -1;
    }

    printf("Socket creado.\n");
    escribir_con_hora(file, "[Servidor] Socket creado.\n");

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    // BIND (the IP/port with socket)
    if (bind(conn_socket, (struct sockaddr *)&server,
             sizeof(server)) == SOCKET_ERROR)
    {
        printf("Error de enlace con el codigo de error: %d", WSAGetLastError());
        escribir_con_hora(file, "[Servidor] Error de enlace.\n");
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }

    printf("Enlace realizado.\n");
    escribir_con_hora(file, "[Servidor] Enlace realizado.\n");

    // LISTEN to incoming connections (socket server moves to listening mode)
    if (listen(conn_socket, 1) == SOCKET_ERROR)
    {
        printf("Escucha fallida con código de error: %d", WSAGetLastError());
        escribir_con_hora(file, "[Servidor] Escucha fallida.\n");
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }

    // ACCEPT incoming connections (server keeps waiting for them)
    printf("Esperando conexiones entrantes...\n");
    int stsize = sizeof(struct sockaddr);
    comm_socket = accept(conn_socket, (struct sockaddr *)&client, &stsize);
    // Using comm_socket is able to send/receive data to/from connected client
    if (comm_socket == INVALID_SOCKET)
    {
        printf("Falló de aceptacion con código de error : %d", WSAGetLastError());
        escribir_con_hora(file, "[Servidor] Falló de aceptacion.\n");
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }
    printf("Conexion entrante desde: %s (%d)\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    char frase[512];
    sprintf(frase, "[Servidor] Conexion entrante desde: %s (%d)\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
    escribir_con_hora(file, frase);

    // Closing the listening sockets (is not going to be used anymore)
    closesocket(conn_socket);

    sqlite3 *DB;
    char *errMsg = 0;
    char *visualizado;
    char *resultados;

    int existe = sqlite3_open("../lib/servidor.db", &DB);
    // //Confirmamos que se abre correctamente
    if (existe != SQLITE_OK)
    {
        printf("Error al abrir la base de datos\n");
        escribir_con_hora(file, "[Servidor] Error al abrir la base de datos.\n");
        // logger con el error
    }

    printf("Base de datos abierta\n");
    escribir_con_hora(file, "[Servidor] Base de datos abierta.\n");

    // SEND and RECEIVE data
    printf("Esperando mensajes entrantes del cliente... \n");
    do
    {
        int bytes = recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
        if (bytes > 0)
        {
            printf("Mensaje recibido... \n");
            printf("Datos recibidos: %s \n", recvBuff);
            if (strcmp(recvBuff, "Visualizar test.") == 0)
            {
                char frase[512];
                sprintf(frase, "[Servidor] Datos recibidos: %s\n", recvBuff);
                escribir_con_hora(file, frase);
                visualizado = visualizar_tests(DB);
                printf("Enviando respuesta... \n");
                strcpy(sendBuff, visualizado);
                send(comm_socket, sendBuff, sizeof(sendBuff), 0);
                char frase2[512];
                sprintf(frase2, "[Servidor] Datos enviados: %s.\n", sendBuff);
                escribir_con_hora(file, frase2);
                printf("Datos enviados: %s \n", sendBuff);
            }
            else if (strcmp(recvBuff, "Crear test.") == 0) {
                // Enviar reconocimiento de recepción al cliente
                strcpy(sendBuff, "Recibido.");
                send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);

                // Recibir los datos del test y preguntas
                bytes = recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
                if (bytes > 0) {
                    printf("Datos recibidos: %s\n", recvBuff);
                    crear_test_y_preguntas(DB, recvBuff);

                    // Enviar confirmación al cliente
                    strcpy(sendBuff, "Test y preguntas creadas exitosamente.");
                    send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);
                }
            }

            else if (strcmp(recvBuff, "Realizar test.") == 0)
            {
                char frase1[512];
                sprintf(frase1, "[Servidor] Datos recibidos: %s\n", recvBuff);
                escribir_con_hora(file, frase1);
                printf("Solicitando test...\n");

                // Recibir nombre del test del cliente
                char test[100];
                recv(comm_socket, test, sizeof(test), 0);
                printf("Test recibido: %s\n", test);
                char frase2[512];
                sprintf(frase2, "[Servidor] Nombre del test elegido: %s\n", test);
                escribir_con_hora(file, frase2);

                int num_ids;
                int *ids = realizarTest(DB, errMsg, test, &num_ids);
                if (ids != NULL)
                {
                    int pCorrect = 0;
                    for (int i = 0; i < num_ids; i++)
                    {
                        char *pregunta = obtenerPregunta(DB, errMsg, ids[i]);
                        if (pregunta == NULL)
                        {
                            printf("Error al obtener la pregunta\n");
                            continue;
                        }

                        int tipo;
                        if (obtenerTipo(DB, errMsg, ids[i], &tipo) != 0)
                        {
                            printf("Error al obtener el tipo de la pregunta\n");
                            free(pregunta);
                            continue;
                        }

                        char frase[512];
                        char *opciones;

                        if (tipo == 1)
                        {
                            opciones = obtenerOpciones(DB, errMsg, ids[i]);
                        }
                        else if (tipo == 2)
                        {
                            opciones = "Verdadero\nFalso\n";
                        }
                        else if (tipo == 3)
                        {
                            opciones = "Pregunta abierta sin opciones.\n";
                        }
                        sprintf(frase, "Pregunta: %s, y sus opciones son:\n %s\n", pregunta, opciones);

                        send(comm_socket, frase, strlen(frase) + 1, 0);
                        printf("Frase enviada: %s \n", frase);
                        char frase4[512];
                        sprintf(frase4, "[Servidor] Frase enviada: %s\n", frase);
                        escribir_con_hora(file, frase4);

                        free(pregunta);

                        recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
                        printf("Respuesta recibida: %s \n", recvBuff);
                        char frase5[512];
                        sprintf(frase5, "[Servidor] Respuesta recibida: %s\n", recvBuff);
                        escribir_con_hora(file, frase5);

                        char *respuesta_correcta = obtenerRespuesta(DB, errMsg, ids[i]);
                        if (respuesta_correcta == NULL)
                        {
                            printf("Error al obtener la respuesta correcta\n");
                            continue;
                        }

                        if (strcmp(recvBuff, respuesta_correcta) == 0)
                        {
                            pCorrect++;
                            send(comm_socket, "Correcto", strlen("Correcto") + 1, 0);
                            printf("Respuesta enviada: Correcto\n");
                            escribir_con_hora(file, "[Servidor] Respuesta enviada: Correcto\n");
                        }
                        else
                        {
                            send(comm_socket, "Incorrecto", strlen("Incorrecto") + 1, 0);
                            printf("Respuesta enviada: Incorrecto\n");
                            escribir_con_hora(file, "[Servidor] Respuesta enviada: Incorrecto\n");
                        }

                        free(respuesta_correcta);
                    }

                    strcpy(sendBuff, "No hay mas preguntas disponibles.");
                    send(comm_socket, sendBuff, sizeof(sendBuff) + 1, 0);
                    escribir_con_hora(file, "[Servidor] No hay más preguntas disponibles\n");

                    printf("Preguntado al cliente si quiere ver la nota.\n");

                    recv(comm_socket, recvBuff,sizeof(recvBuff), 0);

                    if(strcmp(recvBuff, "Ver")==0){
                        char nota[100];
                        sprintf(nota, "%d/%d", pCorrect, num_ids);
                        printf("Nota: %s\n", nota);
                        send(comm_socket, nota, sizeof(nota), 0);
                    }
                    

                    if (guardarResultado(DB, errMsg, test, pCorrect, num_ids) != SQLITE_OK) {
                        printf("Error al guardar el resultado en la base de datos\n");
                    } else {
                        printf("Resultado guardado exitosamente\n");
                    }

                    free(ids);
                }
                else
                {
                    char respuesta[100];
                    strcpy(respuesta, "El test no existe");
                    send(comm_socket, respuesta, strlen(respuesta) + 1, 0);
                    strcat(respuesta,"\n");
                    char frase3[512];
                    sprintf(frase3, "[Servidor] Datos enviados: %s\n", respuesta);
                    escribir_con_hora(file, frase3);
                    printf("Datos enviados: %s \n", respuesta);
                }
            }

            else if (strcmp(recvBuff, "Agregar Pregunta.") == 0)
            {
                char frase[512];
                sprintf(frase, "[Servidor] Datos recibidos: %s\n", recvBuff);
                escribir_con_hora(file, frase);
                char test[100];
                char tipo[100];
                char pregunta[512];
                char opciones[512];
                char respuesta[512];

                recv(comm_socket, tipo, sizeof(tipo), 0);
                char frase1[512];
                sprintf(frase1, "[Servidor] Datos recibidos: %s\n", tipo);
                escribir_con_hora(file, frase1);
                printf("tipo: %s\n", tipo);
                

                if (strcmp(tipo, "0") != 0)
                {
                    recv(comm_socket, pregunta, sizeof(pregunta), 0);
                    char frase2[512];
                    sprintf(frase2, "[Servidor] Datos recibidos: %s\n", pregunta);
                    escribir_con_hora(file, frase2);
                    printf("pregunta: %s\n", pregunta);
                    recv(comm_socket, opciones, sizeof(opciones), 0);
                    char frase3[512];
                    sprintf(frase3, "[Servidor] Datos recibidos: %s\n", opciones);
                    escribir_con_hora(file, frase3);
                    printf("opciones: %s\n", opciones);
                    recv(comm_socket, respuesta, sizeof(respuesta), 0);
                    char frase4[512];
                    sprintf(frase4, "[Servidor] Datos recibidos: %s\n", respuesta);
                    escribir_con_hora(file, frase4);
                    printf("respuesta: %s\n", respuesta);

                    recv(comm_socket, test, sizeof(test), 0);
                    char frase5[512];
                    sprintf(frase5, "[Servidor] Datos recibidos: %s\n", test);
                    escribir_con_hora(file, frase5);
                    printf("test: %s\n", test);

                    anadirPregunta(DB, errMsg, test, tipo, pregunta, opciones, respuesta);
                    printf("Pregunta creada");

                    strcpy(sendBuff, "Pregunta agregada exitosamente.");
                    send(comm_socket, sendBuff, sizeof(sendBuff), 0);
                    char frase6[512];
                    sprintf(frase6, "[Servidor] Datos enviados: %s\n", sendBuff);
                    escribir_con_hora(file, frase6);
                    printf("Datos enviados: %s \n", sendBuff);
                }
                else
                {
                    strcpy(sendBuff, "Retroceder.");
                    send(comm_socket, sendBuff, sizeof(sendBuff), 0);
                    char frase6[512];
                    sprintf(frase6, "[Servidor] Datos enviados: %s\n", sendBuff);
                    escribir_con_hora(file, frase6);
                    printf("Datos enviados: %s \n", sendBuff);
                }
            }

            else if (strcmp(recvBuff, "Eliminar test.") == 0)
            {
                char frase1[512];
                sprintf(frase1, "[Servidor] Datos recibidos: %s\n", recvBuff);
                escribir_con_hora(file, frase1);
                visualizado = visualizar_tests(DB);
                size_t visualizado_len = strlen(visualizado);

                // Envía solo la cantidad de datos necesarios
                send(comm_socket, visualizado, visualizado_len, 0);
                char frase2[512];
                sprintf(frase2, "[Servidor] Datos enviados: %s\n", visualizado);
                escribir_con_hora(file, frase2);
                printf("Datos enviados: %s \n", visualizado);

                memset(recvBuff, 0, sizeof(recvBuff));

                recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
                if (strcmp(recvBuff, "0") == 0)
                {
                    char frase3[512];
                    sprintf(frase3, "[Servidor] Datos recibidos: %s\n", recvBuff);
                    escribir_con_hora(file, frase3);
                    printf("Datos recibidos: %s \n", recvBuff);
                    strcpy(sendBuff, "No se desea eliminar ningun test.");
                    strcat(sendBuff, "\0");
                    send(comm_socket, sendBuff, strlen(sendBuff), 0);
                    char frase4[512];
                    sprintf(frase4, "[Servidor] Datos enviados: %s\n", sendBuff);
                    escribir_con_hora(file, frase4);
                    printf("Datos enviados: %s \n", sendBuff);
                }
                else
                {
                    char frase3[512];
                    sprintf(frase3, "[Servidor] Datos recibidos: %s\n", recvBuff);
                    escribir_con_hora(file, frase3);
                    printf("Datos recibidos: %s \n", recvBuff);

                    eliminar_test(recvBuff, DB, errMsg);

                    strcpy(sendBuff, "Test eliminado correctamente");
                    strcat(sendBuff, "\0"); // Añade un carácter nulo al final de la cadena

                    // Envía solo la cantidad de datos necesarios
                    send(comm_socket, sendBuff, strlen(sendBuff), 0);
                    char frase4[512];
                    sprintf(frase4, "[Servidor] Datos enviados: %s\n", sendBuff);
                    escribir_con_hora(file, frase4);
                    printf("Datos enviados: %s \n", sendBuff);
                }
            }
            else if (strcmp(recvBuff, "resultado teses") == 0)
            {
                char frase1[512];
                sprintf(frase1, "[Servidor] Datos recibidos: %s\n", recvBuff);
                escribir_con_hora(file, frase1);
                resultados = resultado_teses(DB);
                memset(sendBuff, 0, sizeof(sendBuff));
                send(comm_socket, resultados, sizeof(sendBuff), 0);
                char frase2[512];
                sprintf(frase2, "[Servidor] Datos enviados: %s\n", resultados);
                escribir_con_hora(file, frase2);
                printf("Datos enviados: %s \n", resultados);
            }
            else if (strcmp(recvBuff, "Fin") == 0)
            {
                char frase1[512];
                sprintf(frase1, "[Servidor] Datos recibidos: %s\n", recvBuff);
                escribir_con_hora(file, frase1);
                memset(sendBuff, 0, sizeof(sendBuff));
                strcpy(sendBuff, recvBuff);
                send(comm_socket, sendBuff, sizeof(sendBuff), 0);
                char frase2[512];
                sprintf(frase2, "[Servidor] Datos enviados: %s\n", sendBuff);
                escribir_con_hora(file, frase2);
                printf("Datos enviados: %s \n", sendBuff);
                sqlite3_close(DB);
                escribir_con_hora(file, "[Servidor] Cierre de la base de datos.\n");
                break;
            }
        }
    } while (1);

    free(visualizado);
    closesocket(comm_socket);
    escribir_con_hora(file, "[Servidor] Cierre del socket.\n");
    WSACleanup();
    escribir_con_hora(file, "[Servidor] Limpieza del Winsock.\n");
    fclose(file);
    return 0;
}
