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

    char sql[] = "SELECT nombre, cant_preg FROM test";

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
    int first_row = 1;

    printf("\n");
    printf("\n");
    printf("Mostrando tests:\n");
    do
    {
        result = sqlite3_step(stmt);
        if (result == SQLITE_ROW)
        {
            strcpy(nombre, (char *)sqlite3_column_text(stmt, 0));
            cant_preg = sqlite3_column_int(stmt, 1);
            if (first_row)
            {
                sprintf(teses, "%s,%d", nombre, cant_preg);
                first_row = 0;
            }
            else
            {
                sprintf(teses, "%s;%s,%d", teses, nombre, cant_preg);
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

void crearPregunta(sqlite3 *DB, char *errMsg, char tipo, char pregunta, char opciones, char respuesta)
{
    

    char sql[512];
    sprintf(sql, "INSERT INTO pregunta (tipo_preg, pregunta, opciones, respuesta) VALUES ('%s', '%s', '%s', '%s');", tipo, pregunta, opciones, respuesta);
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
            /*else if (strcmp(recvBuff, "Crear Pregunta.") == 0)
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
            }*/
            else if (strcmp(recvBuff, "Eliminar test.") == 0)
            {
                visualizado = visualizar_test(DB, errMsg);
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
