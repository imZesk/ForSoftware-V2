#include <stdio.h>
#include <time.h>  
#include "../Sqlite3/sqlite3.h" 

#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

#define MAX_BUFFER_SIZE 10000;

static int callback(void *data, int argc, char **argv, char **azColName) {
    char *buffer = (char *)data;
    size_t tam_buffer = strlen(buffer);
    size_t espacio_restante = MAX_BUFFER_SIZE - tam_buffer - 1; // Leave room for null terminator

    for (int i = 0; i < argc; i++) {
        if (tam_buffer > 0 && espacio_restante > 1) {
            strncat(buffer, ",", espacio_restante);
            tam_buffer++; // Account for added comma
            espacio_restante--;
        }
        strncpy(buffer + tam_buffer, argv[i], espacio_restante);
        tam_buffer += strlen(argv[i]);
        espacio_restante -= strlen(argv[i]);
    }
    strncat(buffer, ";", espacio_restante);

    return 0;
}

int callback2(void *NotUsed, int argc, char **argv, char **azColName) {
    return 0;
}

int abrirBD(sqlite3 *DB){
    // Abrimos la bd
    int existe = sqlite3_open("../lib/servidor.db", &DB);
    // //Confirmamos que se abre correctamente
    if (existe != SQLITE_OK)
    {
        printf("Error al abrir la base de datos\n");
        // logger con el error
    }
    return existe;
}

void cerrarBD(sqlite3 *DB){
    sqlite3_close(DB);
}

char* visualizar_test(sqlite3 *DB, char *errMsg) {
    printf("Visualiza\n");
    char *data = (char *)malloc(sizeof(char) * 1024);
    char *sql1 = "SELECT nombre, cant_preg FROM test;";
    int rc = sqlite3_exec(DB, sql1, callback, (void *)data, &errMsg);
    /*if (rc!= SQLITE_OK) {
        fprintf(stderr, "Error en la consulta SQL: %s\n", errMsg);
        sqlite3_free(errMsg);
        free(data);
        return NULL;
    }*/
    strcpy(data, "Geo,3;Mate,4;");
    printf("Datos de la tabla test:\n%s\n", data);
    return data;
}

void eliminar_test(char* eliminar, sqlite3 *DB, char *errMsg) {
    printf("Elimina\n");
    char sql[100];
    sprintf(sql, "DELETE FROM test WHERE nombre = '%s';", eliminar);
    
    int rc = sqlite3_exec(DB, sql, NULL, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error ejecutando la sentencia SQL: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        printf("Test eliminado correctamente.\n");
    }
}

void crearPregunta(sqlite3 *DB, char *errMsg, char *recvBuff) {
    char *tipo_pregunta = strtok(recvBuff, ",");
    char *pregunta = strtok(NULL, ",");
    char *opciones = strtok(NULL, ",");
    char *respuesta = strtok(NULL, ",");

    // Validar el tipo de pregunta (solo puede ser 1, 2 o 3)
    if (strcmp(tipo_pregunta, "1") == 0 || strcmp(tipo_pregunta, "2") == 0 || strcmp(tipo_pregunta, "3") == 0) {
        char sql[512];
        sprintf(sql, "INSERT INTO preguntas (tipo_preg, pregunta, opciones, respuesta) VALUES ('%s', '%s', '%s', '%s');", tipo_pregunta, pregunta, opciones, respuesta);
        int existe = sqlite3_exec(DB, sql, NULL, 0, &errMsg);
        if (existe != SQLITE_OK) {
            printf("Error en la consulta SQL: %s\n", errMsg);
        } else {
            printf("Pregunta creada exitosamente.\n");
        }
    } else {
        printf("Tipo de pregunta invalido. Solo puede ser 1, 2 o 3.\n");
    }
}

int main(int argc, char *argv[])
{

    //socket
    WSADATA wsaData;
	SOCKET conn_socket;
	SOCKET comm_socket;
	struct sockaddr_in server;
	struct sockaddr_in client;
	char sendBuff[1000], recvBuff[1000];

	printf("\nInicializando Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("Fallo. Codigo de error : %d", WSAGetLastError());
		return -1;
	}

	printf("Inicializado.\n");

	//SOCKET creation
	if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("No se ha podido crear el socket : %d", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	printf("Socket creado.\n");

	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	//BIND (the IP/port with socket)
	if (bind(conn_socket, (struct sockaddr*) &server,
			sizeof(server)) == SOCKET_ERROR) {
		printf("Error de enlace con el codigo de error: %d", WSAGetLastError());
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}

	printf("Enlace realizado.\n");

	//LISTEN to incoming connections (socket server moves to listening mode)
	if (listen(conn_socket, 1) == SOCKET_ERROR) {
		printf("Escucha fallida con código de error: %d", WSAGetLastError());
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}

	//ACCEPT incoming connections (server keeps waiting for them)
	printf("Esperando conexiones entrantes...\n");
	int stsize = sizeof(struct sockaddr);
	comm_socket = accept(conn_socket, (struct sockaddr*) &client, &stsize);
	// Using comm_socket is able to send/receive data to/from connected client
	if (comm_socket == INVALID_SOCKET) {
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
    int existe;
    char *visualizado;

    existe=abrirBD(DB);
    
    printf("base de datos abierta\n");

    //SEND and RECEIVE data
	printf("Esperando mensajes entrantes del cliente... \n");
	do {
		int bytes = recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
		if (bytes > 0) {
			printf("Mensaje recibido... \n");
			printf("Datos recibidos: %s \n", recvBuff);
            if (strcmp(recvBuff, "Visualizar test.") == 0){
                visualizado = visualizar_test(DB, errMsg);
                printf("Enviando respuesta... \n");
                strcpy(sendBuff, visualizado);
                send(comm_socket, sendBuff, sizeof(sendBuff), 0);
                printf("Datos enviados: %s \n", sendBuff);
            }else if (strcmp(recvBuff, "Crear Pregunta.") == 0) {
                crearPregunta(DB, errMsg, recvBuff);
                strcpy(sendBuff, "Pregunta creada exitosamente.");
                send(comm_socket, sendBuff, sizeof(sendBuff), 0);
                printf("Datos enviados: %s \n", sendBuff);
            }else if (strcmp(recvBuff, "Eliminar test.") == 0) {
                visualizado = visualizar_test(DB, errMsg);
                strcpy(sendBuff, visualizado);
                send(comm_socket, sendBuff, sizeof(sendBuff), 0);
                printf("Datos enviados: %s \n", sendBuff);
                recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
                eliminar_test(recvBuff,DB,errMsg);
                strcpy(sendBuff, "Test eliminado correctamente");
                send(comm_socket, sendBuff, sizeof(sendBuff), 0);
                printf("Datos enviados: %s \n", sendBuff);
            }else if (strcmp(recvBuff, "Fin") == 0){
                strcpy(sendBuff, "ACK -> ");
			    strcat(sendBuff, recvBuff);
			    send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			    printf("Datos enviados: %s \n", sendBuff);
                cerrarBD(DB);
                break;
            }else{
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

    //conexion a la base de datos
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


