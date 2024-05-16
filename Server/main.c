#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"

#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

#define MAX_BUFFER_SIZE 1024

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

void eliminar_test(char *eliminar, sqlite3 *DB, char *errMsg)
{
    printf("Elimina\n");
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
    if (strcmp(opciones, ""))
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

/*void realizarTest(sqlite3 *DB, char *errMsg, char *nombre)
{
    char sql[512];
    int id_t;
    sprintf(sql, "SELECT id_t FROM test WHERE nombre = '%s'", nombre);
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(DB, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Error en la preparación de la consulta SQL para obtener id_t: %s\n", sqlite3_errmsg(DB));
        return;
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
        return;
    }

    sqlite3_finalize(stmt);

    char sql2[512];
    sprintf(sql2, "SELECT id_p FROM tiene WHERE id_t = %d", id_t);
    rc = sqlite3_prepare_v2(DB, sql2, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Error en la preparación de la consulta SQL para obtener las preguntas: %s\n", sqlite3_errmsg(DB));
        return;
    }

    printf("Preguntas:\n");
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int id_p = sqlite3_column_int(stmt, 0);
        char *pregunta = obtenerPregunta(DB, errMsg, id_p);
        if (pregunta != NULL)
        {
            printf("%s\n", pregunta);
            free(pregunta); // Liberar memoria asignada en obtenerPregunta
        }
    }

    if (rc != SQLITE_DONE)
    {
        printf("Error al recuperar las preguntas: %s\n", sqlite3_errmsg(DB));
    }

    sqlite3_finalize(stmt);
}*/

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

int main(int argc, char *argv[])
{

    // socket
    WSADATA wsaData;
    SOCKET conn_socket;
    SOCKET comm_socket;
    struct sockaddr_in server;
    struct sockaddr_in client;
    char sendBuff[1000], recvBuff[1000];

    printf("\nInicializando Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Fallo. Codigo de error : %d", WSAGetLastError());
        return -1;
    }

    printf("Inicializado.\n");

    // SOCKET creation
    if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("No se ha podido crear el socket : %d", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    printf("Socket creado.\n");

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    // BIND (the IP/port with socket)
    if (bind(conn_socket, (struct sockaddr *)&server,
             sizeof(server)) == SOCKET_ERROR)
    {
        printf("Error de enlace con el codigo de error: %d", WSAGetLastError());
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }

    printf("Enlace realizado.\n");

    // LISTEN to incoming connections (socket server moves to listening mode)
    if (listen(conn_socket, 1) == SOCKET_ERROR)
    {
        printf("Escucha fallida con código de error: %d", WSAGetLastError());
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
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }
    printf("Conexion entrante desde: %s (%d)\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    // Closing the listening sockets (is not going to be used anymore)
    closesocket(conn_socket);

    sqlite3 *DB;
    char *errMsg = 0;
    char *visualizado;

    int existe = sqlite3_open("../lib/servidor.db", &DB);
    // //Confirmamos que se abre correctamente
    if (existe != SQLITE_OK)
    {
        printf("Error al abrir la base de datos\n");
        // logger con el error
    }

    printf("base de datos abierta\n");

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
                visualizado = visualizar_tests(DB);
                printf("Enviando respuesta... \n");
                strcpy(sendBuff, visualizado);
                send(comm_socket, sendBuff, sizeof(sendBuff), 0);
                printf("Datos enviados: %s \n", sendBuff);
            }
            else if (strcmp(recvBuff, "Realizar test.") == 0)
            {
                printf("Solicitando test...\n");

                // Recibir nombre del test del cliente
                recv(comm_socket, recvBuff, sizeof(recvBuff), 0);

                int num_ids;
                int *ids = realizarTest(DB, errMsg, recvBuff, &num_ids);
                if (ids != NULL)
                {
                    for (int i = 0; i < num_ids; i++)
                    {
                        char *pregunta = obtenerPregunta(DB, errMsg, ids[i]);
                        if (pregunta == NULL)
                        {
                            printf("Error al obtener la pregunta\n");
                            continue;
                        }

                        send(comm_socket, pregunta, strlen(pregunta) + 1, 0);
                        printf("Pregunta enviada: %s \n", pregunta);

                        free(pregunta);

                        recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
                        printf("Respuesta recibida: %s \n", recvBuff);

                        char *respuesta_correcta = obtenerRespuesta(DB, errMsg, ids[i]);
                        if (respuesta_correcta == NULL)
                        {
                            printf("Error al obtener la respuesta correcta\n");
                            continue;
                        }

                        if (strcmp(recvBuff, respuesta_correcta) == 0)
                        {
                            send(comm_socket, "Correcto", strlen("Correcto") + 1, 0);
                            printf("Respuesta enviada: Correcto\n");
                        }
                        else
                        {
                            send(comm_socket, "Incorrecto", strlen("Incorrecto") + 1, 0);
                            printf("Respuesta enviada: Incorrecto\n");
                        }

                        free(respuesta_correcta);
                    }

                    strcpy(sendBuff, "No hay más preguntas disponibles.");
                    send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);

                    free(ids);
                }
                else
                {
                    char respuesta[100];
                    strcpy(respuesta, "El test no existe");
                    send(comm_socket, respuesta, strlen(respuesta) + 1, 0);
                    printf("Datos enviados: %s \n", respuesta);
                }
            }

            else if (strcmp(recvBuff, "Crear Pregunta.") == 0)
            {
                char tipo[10];
                char pregunta[100];
                char opciones[100];
                char respuesta[100];

                recv(comm_socket, tipo, sizeof(tipo), 0);
                recv(comm_socket, pregunta, sizeof(pregunta), 0);
                recv(comm_socket, opciones, sizeof(opciones), 0);
                recv(comm_socket, respuesta, sizeof(respuesta), 0);

                // Llamar a la función crearPregunta con los datos recibidos
                crearPregunta(DB, errMsg, tipo, pregunta, opciones, respuesta);

                strcpy(sendBuff, "Pregunta creada exitosamente.");
                send(comm_socket, sendBuff, sizeof(sendBuff), 0);
                printf("Datos enviados: %s \n", sendBuff);
            }
            else if (strcmp(recvBuff, "Eliminar test.") == 0)
            {
                visualizado = visualizar_tests(DB);
                size_t visualizado_len = strlen(visualizado);

                // Envía solo la cantidad de datos necesarios
                send(comm_socket, visualizado, visualizado_len, 0);
                printf("Datos enviados: %s \n", visualizado);

                memset(recvBuff, 0, sizeof(recvBuff));

                recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
                printf("Datos recibidos: %s \n", recvBuff);

                eliminar_test(recvBuff, DB, errMsg);


                strcpy(sendBuff, "Test eliminado correctamente");
                strcat(sendBuff, "\0"); // Añade un carácter nulo al final de la cadena

                // Envía solo la cantidad de datos necesarios
                send(comm_socket, sendBuff, strlen(sendBuff), 0);
                printf("Datos enviados: %s \n", sendBuff);
            }
            else if (strcmp(recvBuff, "Fin") == 0)
            {
                strcpy(sendBuff, "ACK -> ");
                strcat(sendBuff, recvBuff);
                send(comm_socket, sendBuff, sizeof(sendBuff), 0);
                printf("Datos enviados: %s \n", sendBuff);
                sqlite3_close(DB);
                break;
            }
            else
            {
                printf("Enviando respuesta... \n");
                strcpy(sendBuff, "ACK -> ");
                strcat(sendBuff, recvBuff);
                send(comm_socket, sendBuff, sizeof(sendBuff), 0);
                printf("Datos enviados: %s \n", sendBuff);
            }
        }
    } while (1);

    free(visualizado);
    closesocket(comm_socket);
    WSACleanup();
    return 0;
}

// conexion a la base de datos
/*sqlite3 *DB;
char *errMsg = 0;

// Abrimos la bd
int existe = sqlite3_open("./lib/Preguntas.db", &DB);
// //Confirmamos que se abre correctamente
if (existe != SQLITE_OK)
{
    printf("Error");
    // logger con el error
    return 1;
}
// realizar test
char sql1[1000];
// ver test
char *sql2 = "SELECT nombre, pregunta, respuesta FROM test;";
// eliminar test
char sql3[1000];
// ver nombres
char *sql4 = "SELECT nombre FROM test;";
// ver la respuesta
char sql5[1000];
// visualizar nota
char *sql6 = "Select respuesta, preguntas_realizadas from resultado";
// añadir pregunta a test
char sql7[1000];
// visualizar preguntas
char sql8 = "SELECT * from preguntas";

existe = sqlite3_exec(DB, sql4, callback, 0, &errMsg);

if (existe != SQLITE_OK)
{
    // fprintf(stderr, "[%d:%d:%d] Error en la consulta SQL: %s\n", tm.tm_hour,tm.tm_min,tm.tm_sec, errMsg);
    sqlite3_free(errMsg);
}
printf("Nombre del test a hacer: ");

fflush(stdout);
fflush(stdin);

// sprintf(sql1, "SELECT pregunta FROM pregunta WHERE nombre LIKE '%s';", hacer);

existe = sqlite3_exec(DB, sql1, callback, 0, &errMsg);

if (existe != SQLITE_OK)
{
    sqlite3_free(errMsg);
}
printf("Responda la pregunta: ");

fflush(stdout);
fflush(stdin);

// sprintf(sql5, "SELECT respuesta FROM pregunta WHERE nombre LIKE '%s';", hacer);

existe = sqlite3_exec(DB, sql5, callback, 0, &errMsg);

if (existe != SQLITE_OK)
{
    sqlite3_free(errMsg);
}

existe = sqlite3_exec(DB, sql2, callback, 0, &errMsg);

if (existe != SQLITE_OK)
{

    sqlite3_free(errMsg);
}
existe = sqlite3_exec(DB, sql4, callback, 0, &errMsg);

if (existe != SQLITE_OK)
{
    sqlite3_free(errMsg);
}
printf("Nombre del test a eliminar: ");

fflush(stdout);
fflush(stdin);

// sprintf(sql3, "DELETE FROM pregunta WHERE nombre = '%s';", eliminar);

existe = sqlite3_exec(DB, sql3, 0, 0, NULL);
if (existe != SQLITE_OK)
{
    sqlite3_free(errMsg);
}
sqlite3_close(DB);*/

// CLOSING the sockets and cleaning Winsock...
