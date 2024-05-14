#include <iostream>
#include <winsock2.h>
#include <cstring>
#include "menus.h"
#include <limits>
#include <sstream>
#include <string>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

using namespace std;

void separarPalabras(const char *cadena)
{
	stringstream ss(cadena);
	string grupo;

	// Iterar sobre cada grupo separado por punto y coma
	while (getline(ss, grupo, ';'))
	{
		stringstream grupo_ss(grupo);
		string palabra;
		int contador = 0;

		// Iterar sobre cada palabra en el grupo separado por coma
		while (getline(grupo_ss, palabra, ','))
		{
			// Imprimir cada palabra en una fila diferente
			if (contador == 0)
			{
				cout << "Nombre: " << palabra << endl;
			}
			else if (contador == 1)
			{
				cout << "Cantidad de preguntas: " << palabra << endl;
			}
			else
			{
				cout << palabra << endl;
			}
			contador++;
		}

		// Imprimir una línea en blanco entre grupos
		cout << endl;
	}
}

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
	char tipo[10];
	char pregunta[100];
	char opciones[100];
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
			cout << endl;
			cout << "Teses: " << endl;
			separarPalabras(recvBuff);
			break;

		case '4':
			cout << "Envio del mensaje 1..." << endl;
			strcpy(sendBuff, "Eliminar test.");
			send(s, sendBuff, strlen(sendBuff) + 1, 0);

			cout << "Recepcion del mensaje 1..." << endl;
			recv(s, recvBuff, sizeof(recvBuff), 0);
			cout << "Datos recibidos: " << recvBuff << endl;
			cout << endl;
			cout << "Teses: " << endl;
			separarPalabras(recvBuff);
			cout << "Nombre del test a eliminar: ";
			cin >> eliminar;
			cout << endl;
			strcpy(sendBuff, eliminar);
			send(s, sendBuff, strlen(sendBuff) + 1, 0);
			cout << "Recepcion del mensaje 2..." << endl;
			recv(s, recvBuff, sizeof(recvBuff), 0);
			cout << "Datos recibidos: " << recvBuff << endl;
			break;

		case '5':
			cout << "Envio del mensaje 1..." << endl;
			strcpy(sendBuff, "Crear Pregunta.");
			send(s, sendBuff, strlen(sendBuff) + 1, 0);

			do
			{
				cout << "Ingrese el tipo de pregunta (1, 2 o 3): ";
				cin.getline(tipo, sizeof(tipo));
			} while (strcmp(tipo, "1") != 0 && strcmp(tipo, "2") != 0 && strcmp(tipo, "3") != 0);

			cout << "Ingrese la pregunta: ";
			cin.getline(pregunta, sizeof(pregunta));

			if (strcmp(tipo, "1") == 0)
			{
				cout << "Ingrese las opciones (opcional): ";
				cin.getline(opciones, sizeof(opciones));
			}
			else
			{
				do
				{
					cout << "Ingrese las opciones: ";
					cin.getline(opciones, sizeof(opciones));
				} while (strcmp(opciones, ""));
			}

			cout << "Ingrese la respuesta: ";
			cin.getline(respuesta, sizeof(respuesta));

			send(s, tipo, sizeof(tipo), 0);
    		send(s, pregunta, sizeof(pregunta), 0);
    		send(s, opciones, sizeof(opciones), 0);
    		send(s, respuesta, sizeof(respuesta), 0);

			cout << "Recepcion del mensaje..." << endl;
			recv(s, recvBuff, sizeof(recvBuff), 0);
			cout << "Datos recibidos: " << recvBuff << endl;
			break;
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