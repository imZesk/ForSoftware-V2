#include <iostream>
#include <winsock2.h>
#include <cstring>
#include "menus.h"
#include <limits>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

using namespace std;

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	SOCKET s;
	struct sockaddr_in server;
	char sendBuff[512], recvBuff[512];

	cout << endl
		 << "Inicializando Winsock..." << endl;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "Fallo. Codigo de error : " << WSAGetLastError() << endl;
		return -1;
	}

	cout << "Inicializado." << endl;

	// SOCKET creation
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		cout << "No se ha podido crear el socket :" << WSAGetLastError() << endl;
		WSACleanup();
		return -1;
	}

	cout << "Socket creado." << endl;

	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	// CONNECT to remote server
	if (connect(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		cout << "Error de conexion: " << WSAGetLastError() << endl;
		closesocket(s);
		WSACleanup();
		return -1;
	}
	cout << "Conexion establecida con: " << inet_ntoa(server.sin_addr) << " ("
		 << ntohs(server.sin_port) << ")" << endl;

	menuPrincipal();

	// Menu:
	int contador;
	char opcion;
	char eliminar[100];
	char hacer[100];
	char respuesta[100];
	do
	{
		opcion = menuPrincipal();

		switch (opcion)
		{
		case '1':
			cout << "Envio del mensaje 1..." << endl;
			strcpy(sendBuff, "Crear test.");
			send(s, sendBuff, strlen(sendBuff) + 1, 0);

			cout << "Recepcion del mensaje 1..." << endl;
			recv(s, recvBuff, sizeof(recvBuff), 0);
			cout << "Datos recibidos: " << recvBuff << endl;
			break;

		case '2':
			cout << "Envio del mensaje 1..." << endl;
			strcpy(sendBuff, "Realizar test.");
			send(s, sendBuff, strlen(sendBuff) + 1, 0);

			cout << "Recepcion del mensaje 1..." << endl;
			recv(s, recvBuff, sizeof(recvBuff), 0);
			cout << "Datos recibidos: " << recvBuff << endl;
			break;

		case '3':
			cout << "Envio del mensaje 1..." << endl;
			strcpy(sendBuff, "Visualizar test.");
			send(s, sendBuff, strlen(sendBuff) + 1, 0);

			cout << "Recepcion del mensaje 1..." << endl;
			recv(s, recvBuff, sizeof(recvBuff), 0);
			cout << "Datos recibidos: " << recvBuff << endl;
			break;

		case '4':
			cout << "Envio del mensaje 1..." << endl;
			strcpy(sendBuff, "Eliminar test.");
			send(s, sendBuff, strlen(sendBuff) + 1, 0);

			cout << "Recepcion del mensaje 1..." << endl;
			recv(s, recvBuff, sizeof(recvBuff), 0);
			cout << "Datos recibidos: " << recvBuff << endl;
			break;

		case '5':
			cout << "Ingrese el tipo de pregunta (1, 2 o 3): ";
			cin.getline(hacer, sizeof(hacer));
			cout << "Ingrese la pregunta: ";
			cin.getline(sendBuff, sizeof(sendBuff));
			cout << "Ingrese las opciones (opcional): ";
			cin.getline(eliminar, sizeof(eliminar));
			cout << "Ingrese la respuesta: ";
			cin.getline(respuesta, sizeof(respuesta));

			// Validar el tipo de pregunta (solo puede ser 1, 2 o 3)
			if (strcmp(hacer, "1") == 0 || strcmp(hacer, "2") == 0 || strcmp(hacer, "3") == 0)
			{
				// Formatear el mensaje y enviarlo al servidor
				strcpy(sendBuff, "Crear Pregunta.");
				strcat(sendBuff, hacer);
				strcat(sendBuff, ",");
				strcat(sendBuff, sendBuff);
				strcat(sendBuff, ",");
				strcat(sendBuff, eliminar);
				strcat(sendBuff, ",");
				strcat(sendBuff, respuesta);
				send(s, sendBuff, strlen(sendBuff) + 1, 0);

				cout << "Recepcion del mensaje..." << endl;
				recv(s, recvBuff, sizeof(recvBuff), 0);
				cout << "Datos recibidos: " << recvBuff << endl;
			}
			else
			{
				cout << "Tipo de pregunta invalido. Solo puede ser 1, 2 o 3." << endl;
			}

		case '0':
			cout << "Envio del mensaje 1..." << endl;
			strcpy(sendBuff, "Fin");
			send(s, sendBuff, strlen(sendBuff) + 1, 0);

			cout << "Recepcion del mensaje 1..." << endl;
			recv(s, recvBuff, sizeof(recvBuff), 0);
			cout << "Datos recibidos: " << recvBuff << endl;

			cout << "Fin del programa" << endl;
			break;

		default:
			cout << "ERROR, introduce de nuevo" << endl;
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	} while (opcion != '0');

	// CLOSING the socket and cleaning Winsock...
	closesocket(s);
	WSACleanup();

	return 0;
}