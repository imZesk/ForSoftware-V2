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

char *resultado_teses(sqlite3 *db)
{
    sqlite3_stmt *stmt;

    char sql[] = "SELECT test.nombre,test.id_t,(SELECT resultado.id_r FROM resultado WHERE resultado.id_t = test.id_t) AS id_r,(SELECT resultado.pregs_realiz FROM resultado WHERE resultado.id_t = test.id_t) AS pregs_realiz,(SELECT resultado.respuestas_c FROM resultado WHERE resultado.id_t = test.id_t) AS respuestas_c FROM test;";

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
            preg_reali = sqlite3_column_int(stmt, 1);
            respuestas_correct = sqlite3_column_int(stmt, 2);
            if (first_row)
            {
                sprintf(resultado, "%s,%d,%d", nombre, preg_reali, respuestas_correct);
                first_row = 0;
            }
            else
            {
                sprintf(resultado, "%s;%s,%d,%d", resultado, nombre, preg_reali, respuestas_correct);
            }
        }
    } while (result == SQLITE_ROW);

    printf("Resultados: %s\n", resultado);

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

char **obtenerRespuestas(sqlite3 *DB, char *errMsg, int id_p, int *num_respuestas)
{
    char sql[512];
    sprintf(sql, "SELECT respuesta FROM respuesta WHERE id_p = %d", id_p);
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(DB, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Error en la preparación de la consulta SQL para obtener las respuestas: %s\n", sqlite3_errmsg(DB));
        return NULL;
    }

    // Contar cuántas respuestas hay
    *num_respuestas = 0;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        (*num_respuestas)++;
    }
    sqlite3_reset(stmt);

    if (*num_respuestas == 0)
    {
        sqlite3_finalize(stmt);
        return NULL; // No hay respuestas
    }

    // Asignar memoria para las respuestas
    char **respuestas = malloc(*num_respuestas * sizeof(char *));
    if (respuestas == NULL)
    {
        printf("Error al asignar memoria para las respuestas\n");
        sqlite3_finalize(stmt);
        return NULL;
    }

    // Recuperar las respuestas
    int index = 0;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        const unsigned char *text = sqlite3_column_text(stmt, 0);
        respuestas[index] = malloc((strlen((const char *)text) + 1) * sizeof(char));
        if (respuestas[index] == NULL)
        {
            printf("Error al asignar memoria para una respuesta\n");
            for (int i = 0; i < index; i++)
            {
                free(respuestas[i]);
            }
            free(respuestas);
            sqlite3_finalize(stmt);
            return NULL;
        }
        strcpy(respuestas[index], (const char *)text);
        index++;
    }

    sqlite3_finalize(stmt);
    return respuestas;
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
    char *resultados;

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

                        int tipo;
                        if (obtenerTipo(DB, errMsg, ids[i], &tipo) != 0)
                        {
                            printf("Error al obtener el tipo de la pregunta\n");
                            free(pregunta);
                            continue;
                        }

                        char frase[512];
                        sprintf(frase, "Pregunta: %s, y sus opciones son:\n", pregunta);

                        if (tipo == 1)
                        {
                            int num_respuestas;
                            char **respuestas = obtenerRespuestas(DB, errMsg, ids[i], &num_respuestas);
                            if (respuestas != NULL)
                            {
                                for (int j = 0; j < num_respuestas; j++)
                                {
                                    strcat(frase, respuestas[j]);
                                    strcat(frase, "\n");
                                    free(respuestas[j]);
                                }
                                free(respuestas);
                            }
                        }
                        else if (tipo == 2)
                        {
                            strcat(frase, "Verdadero\nFalso\n");
                        }
                        else if (tipo == 3)
                        {
                            strcat(frase, "Pregunta abierta sin opciones.\n");
                        }

                        send(comm_socket, frase, strlen(frase) + 1, 0);
                        printf("Frase enviada: %s \n", frase);

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
                    send(comm_socket, sendBuff, sizeof(sendBuff) + 1, 0);

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
                char test[100];
                char tipo;

                recv(comm_socket, test, sizeof(test), 0);
                recv(comm_socket, &tipo, sizeof(tipo), 0);

                if (tipo != '0')
                {
                    char pregunta[512];
                    char opciones[512];
                    char respuesta[512];

                    recv(comm_socket, pregunta, sizeof(pregunta), 0);
                    recv(comm_socket, opciones, sizeof(opciones), 0);
                    recv(comm_socket, respuesta, sizeof(respuesta), 0);

                    anadirPregunta(DB, errMsg, test, &tipo, pregunta, opciones, respuesta);
                    printf("Pregunta creada");

                    strcpy(sendBuff, "Pregunta agregada exitosamente.");
                    send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);
                    printf("Datos enviados: %s \n", sendBuff);
                }
                else
                {
                    strcpy(sendBuff, "Retroceder.");
                    send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);
                    printf("Datos enviados: %s \n", sendBuff);
                }
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
                if (strcmp(recvBuff, "0") == 0)
                {
                    printf("Datos recibidos: %s \n", recvBuff);
                    strcpy(sendBuff, "No se desea eliminar ningun test.");
                    strcat(sendBuff, "\0");
                    send(comm_socket, sendBuff, strlen(sendBuff), 0);
                    printf("Datos enviados: %s \n", sendBuff);
                }
                else
                {
                    printf("Datos recibidos: %s \n", recvBuff);

                    eliminar_test(recvBuff, DB, errMsg);

                    strcpy(sendBuff, "Test eliminado correctamente");
                    strcat(sendBuff, "\0"); // Añade un carácter nulo al final de la cadena

                    // Envía solo la cantidad de datos necesarios
                    send(comm_socket, sendBuff, strlen(sendBuff), 0);
                    printf("Datos enviados: %s \n", sendBuff);
                }
            }
            else if (strcmp(recvBuff, "resultado teses") == 0)
            {
                resultados = resultado_teses(DB);
                memset(sendBuff, 0, sizeof(sendBuff));
                send(comm_socket, resultados, sizeof(sendBuff), 0);
                printf("Datos enviados: %s \n", sendBuff);
                break;
            }
            else if (strcmp(recvBuff, "Fin") == 0)
            {
                memset(sendBuff, 0, sizeof(sendBuff));
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
