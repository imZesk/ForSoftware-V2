#include <iostream>
#include <winsock2.h>
#include <cstring>
#include "menus.h"
#include "Encuesta.h"
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
		int id_encuesta = 0;
		string nombre_encuesta;
		int cantidad_preguntas = 0;

		int contador = 0;
		while (getline(grupo_ss, palabra, ','))
		{
			if (contador == 0)
			{
				id_encuesta = stoi(palabra);
			}
			else if (contador == 1)
			{
				nombre_encuesta = palabra;
			}
			else
			{
				cantidad_preguntas = stoi(palabra);
			}
			contador++;
		}

		// Crear una nueva encuesta con los valores obtenidos
		Encuesta nueva_encuesta(id_encuesta, cantidad_preguntas, nombre_encuesta);
		cout << "Nombre: " << nueva_encuesta.nombre << endl;
		cout << "Cantidad de preguntas: " << nueva_encuesta.cantidadPreguntas << endl;
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
	char opcion1[100];
	char opcion2[100];
	char opcion3[100];
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
			cout << "Realizando test..." << endl;
			strcpy(sendBuff, "Realizar test.");
			send(s, sendBuff, strlen(sendBuff) + 1, 0);

			while (true)
			{
				// Recibir pregunta del servidor
				recv(s, recvBuff, sizeof(recvBuff), 0);
				if (strcmp(recvBuff, "No hay preguntas") == 0)
				{
					cout << "No hay más preguntas disponibles." << endl;
					break;
				}
				else
				{
					cout << "Pregunta recibida: " << recvBuff << endl;

					// Enviar respuesta al servidor
					string respuesta;
					cout << "Ingrese su respuesta: ";
					getline(cin, respuesta);
					strcpy(sendBuff, respuesta.c_str());
					send(s, sendBuff, strlen(sendBuff) + 1, 0);

					// Recibir resultado del servidor
					recv(s, recvBuff, sizeof(recvBuff), 0);
					cout << "Resultado: " << recvBuff << endl;
				}
			}

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
				cout << "Ingrese el tipo de pregunta (1(pregunta con 3 opciones), 2(Verdadero o Falso) o 3(Pregunta abierta)): ";
				cin.getline(tipo, sizeof(tipo));
			} while (strcmp(tipo, "1") != 0 && strcmp(tipo, "2") != 0 && strcmp(tipo, "3") != 0);

			cout << "Ingrese la pregunta: ";
			cin.getline(pregunta, sizeof(pregunta));

			if (strcmp(tipo, "2") == 0)
			{
				strcpy(opciones, "Verdadero, Falso");
			}
			else if (strcmp(tipo, "1") == 0)
			{
				do
				{
					cout << "Ingrese la primera opcion opciones: ";
					cin.getline(opcion1, sizeof(opcion1));
				} while (strcmp(opcion1, "") == 0);
				do
				{
					cout << "Ingrese la segunda opcion opciones: ";
					cin.getline(opcion2, sizeof(opcion2));
				} while (strcmp(opcion2, "") == 0);
				do
				{
					cout << "Ingrese la tercera opcion opciones: ";
					cin.getline(opcion3, sizeof(opcion3));
				} while (strcmp(opcion3, "") == 0);

				strcpy(opciones, opcion1);
				strcat(opciones, ", ");
				strcat(opciones, opcion2);
				strcat(opciones, ", ");
				strcat(opciones, opcion3);
			}
			else
			{
				strcpy(opciones, NULL);
			}

			if (strcmp(tipo, "3") == 0)
			{
				cout << "Ingrese la respuesta: ";
				cin.getline(respuesta, sizeof(respuesta));
			}
			else if (strcmp(tipo, "2") == 0)
			{
				do
				{
					cout << "Ingrese la respuesta(1 para verdadero o 0 para falso): ";
					cin.getline(respuesta, sizeof(respuesta));
				} while (strcmp(respuesta, "1") != 0 && strcmp(respuesta, "2") != 0);
				if (strcmp(respuesta, "1") == 0)
				{
					strcpy(respuesta, "Verdadero");
				}
				else
				{
					strcpy(respuesta, "Falso");
				}
			}
			else
			{
				do
				{
					cout << "Ingrese la respuesta(1, 2, 3): ";
					cin.getline(respuesta, sizeof(respuesta));
				} while (strcmp(respuesta, "1") != 0 && strcmp(respuesta, "2") != 0 && strcmp(respuesta, "3") != 0);
				if (strcmp(respuesta, "1") == 0)
				{
					strcpy(respuesta, opcion1);
				}
				else if (strcmp(respuesta, "2") == 0)
				{
					strcpy(respuesta, opcion2);
				}
				else
				{
					strcpy(respuesta, opcion3);
				}
			}

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