#include <stdio.h>
#include <time.h>  
#include "../Sqlite3/sqlite3.h" 

#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

static int callback(void *data, int argc, char **argv, char **azColName)
{
    for (int i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
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

    // Abrimos la bd
    int existe = sqlite3_open("../lib/servidor.db", &DB);
    // //Confirmamos que se abre correctamente
    if (existe != SQLITE_OK)
    {
        printf("Error");
        // logger con el error
        return 1;
    }
    printf("base de datos abierta\n");

    //SEND and RECEIVE data
	printf("Esperando mensajes entrantes del cliente... \n");
	do {
		int bytes = recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
		if (bytes > 0) {
			printf("Mensaje recibido... \n");
			printf("Datos recibidos: %s \n", recvBuff);
            if (strcmp(recvBuff, "Visualizar test.") == 0){
                char visualizado=visualizar_test();
                printf("Enviando respuesta... \n");
			    strcpy(sendBuff, visualizado);
			    send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			    printf("Datos enviados: %s \n", sendBuff);
            }

			printf("Enviando respuesta... \n");
			strcpy(sendBuff, "ACK -> ");
			strcat(sendBuff, recvBuff);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			printf("Datos enviados: %s \n", sendBuff);

			if (strcmp(recvBuff, "Fin") == 0)
				break;
		}
	} while (1);

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
	closesocket(comm_socket);
	WSACleanup();
    return 0;
}

char visualizar_test(){

}