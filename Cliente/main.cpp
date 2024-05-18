#include <iostream>
#include <winsock2.h>
#include <cstring>
#include "menus.h"
#include "Pregunta/encuesta.h"
#include <limits>
#include <sstream>
#include <string>
#include <list>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

using namespace std;

list<string> separarPalabras(const char *cadena)
{
	stringstream ss(cadena);
	string grupo;
	list<string> nombresEncuestas;

	// Iterar sobre cada grupo separado por punto y coma
	while (getline(ss, grupo, ';'))
	{
		stringstream grupo_ss(grupo);
		string palabra;
		int id_encuesta = 0;
		string nombre_encuesta;
		const char *nombre;
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

		nombre = nombre_encuesta.c_str();
		// Crear una nueva encuesta con los valores obtenidos
		Encuesta nueva_encuesta(cantidad_preguntas);
		nueva_encuesta.setNombre(nombre);
		nueva_encuesta.setId(id_encuesta);
		nombresEncuestas.push_back(nueva_encuesta.getNombre());
		cout << "Nombre: " << nueva_encuesta.getNombre() << endl;
		cout << "Cantidad de preguntas: " << nueva_encuesta.getCantidadPreguntas() << endl;
		cout << endl;
	}
	return nombresEncuestas;
}

void separarResultado(const char *cadena)
{
	stringstream ss(cadena);
	string grupo;

	// Iterar sobre cada grupo separado por punto y coma
	while (getline(ss, grupo, ';'))
	{
		stringstream grupo_ss(grupo);
		string palabra;
		string nombre_encuesta;
		int preg_realiza = 0;
		int respuestas_c = 0;
		int contador = 0;
		const char *nombre;
		while (getline(grupo_ss, palabra, ','))
		{
			if (contador == 0)
			{
				nombre_encuesta = palabra;
			}
			else if (contador == 1)
			{
				preg_realiza = stoi(palabra);
			}
			else
			{
				respuestas_c = stoi(palabra);
			}
			contador++;
		}

		nombre = nombre_encuesta.c_str();
		// Crear una nueva encuesta con los valores obtenidos
		Encuesta nueva_encuesta(preg_realiza);
		nueva_encuesta.setNombre(nombre);
		nueva_encuesta.setNota(respuestas_c);
		cout << "Nombre: " << nueva_encuesta.getNombre() << nueva_encuesta.getNota() << "/" << nueva_encuesta.getCantidadPreguntas() << endl;
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


	// Menu:
	int contador;
	char opcion;
	char eliminar[100];
	char hacer[100];
	char tipo[100];
	char pregunta[100];
	char opciones[100];
	char opcion1[100];
	char opcion2[100];
	char opcion3[100];
	char respuesta[100];
	char test[100];
	list<string> nombresEncuestas;
	bool encontrado = false;
	do
	{
		opcion = menuPrincipal();

		switch (opcion)
		{
		case '1':

			char nombreEncuesta[100];
			cout << "Envio del mensaje 1..." << endl;
			strcpy(sendBuff, "Crear test.");
			send(s, sendBuff, strlen(sendBuff) + 1, 0);

			cout << "Recepcion del mensaje 1..." << endl;
			recv(s, recvBuff, sizeof(recvBuff), 0);
			cout << "Datos recibidos: " << recvBuff << endl;
			fflush(stdout);
			fflush(stdin);
			cout << "Introduce el nombre de la encuesta: ";
			fflush(stdout);
			fflush(stdin);
			cin.getline(nombreEncuesta, sizeof(nombreEncuesta));
			fflush(stdout);
			fflush(stdin);
			cout << "Ingresaste: " << nombreEncuesta << endl;
			fflush(stdout);
			fflush(stdin);

			break;

		case '2':
			cout << "Realizando test..." << endl;
			strcpy(sendBuff, "Realizar test.");
			send(s, sendBuff, strlen(sendBuff) + 1, 0);

			cout << "Que test desea realizar: ";
			cin >> sendBuff;
			send(s, sendBuff, strlen(sendBuff) + 1, 0);

			while (true)
			{
				// Recibir pregunta y opciones del servidor
				recv(s, recvBuff, sizeof(recvBuff) + 1, 0);

				if (strcmp(recvBuff, "No hay más preguntas disponibles.") == 0)
				{
					cout << "No hay más preguntas disponibles." << endl;
					break;
				}
				else if (strcmp(recvBuff, "El test no existe") == 0)
				{
					cout << "El test no existe" << endl;
					break;
				}
				else
				{
					cout << recvBuff << endl; // Mostrar la pregunta y las opciones

					cout << "Ingrese su respuesta: ";
					cin >> respuesta;
					strcpy(sendBuff, respuesta);
					send(s, sendBuff, strlen(sendBuff) + 1, 0);

					// Recibir resultado del servidor
					recv(s, recvBuff, sizeof(recvBuff), 0);
					cout << "Resultado: " << recvBuff << endl;
				}
			}
			send(s, "Listo", strlen("Listo") + 1, 0);

			char nota[100];

			recv(s, nota, sizeof(nota), 0);
			cout << "Tu nota es: " << nota << endl;
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
			nombresEncuestas = separarPalabras(recvBuff);
			do
			{
				cout << "Nombre del test a eliminar (0 si no quieres eliminar): ";
				cin >> eliminar;
				cout << endl;

				string palabraCompararStr(eliminar);
				for (const string &nombre : nombresEncuestas)
				{
					if (nombre == palabraCompararStr)
					{
						encontrado = true;
						break;
					}
				}
				if (strcmp(eliminar, "0") == 0)
				{
					cout << "No se desea eliminar ningun test." << endl;
					break;
				}
				else if (!encontrado)
				{
					cout << "El nombre '" << eliminar << "' no coincide con ningun nombre de los teses. Intentalo de nuevo." << endl;
				}
			} while (!encontrado);

			memset(recvBuff, 0, sizeof(recvBuff));
			strcpy(sendBuff, eliminar);
			send(s, sendBuff, strlen(sendBuff) + 1, 0);
			cout << "Recepcion del mensaje 2..." << endl;
			recv(s, recvBuff, sizeof(recvBuff), 0);
			cout << "Datos recibidos: " << recvBuff << endl;
			break;

		case '5':
			cout << "Envio del mensaje 1..." << endl;
			strcpy(sendBuff, "Agregar Pregunta.");
			send(s, sendBuff, strlen(sendBuff) + 1, 0);

			do
			{
				cout << "Ingrese el tipo de pregunta\n1: pregunta con 3 opciones\n2: Verdadero o Falso\n3: Pregunta abierta\n0 si no quieres añadir ninguna pregunta: ";
				cin>>tipo;
			} while (strcmp(tipo, "1") != 0 && strcmp(tipo, "2") != 0 && strcmp(tipo, "3") != 0 && strcmp(tipo, "0") != 0);

			send(s, tipo, strlen(tipo) + 1, 0);

			if (strcmp(tipo, "0") != 0)
			{
				cout << "Ingrese la pregunta: ";
				cin.ignore();
				string pregunta;
    			getline(cin, pregunta);

				char preg[512];
				strcpy(preg, pregunta.c_str());

				cout<<preg<<endl;

				send(s, preg, strlen(preg) + 1, 0);

				if (strcmp(tipo, "2") == 0)
				{
					strcpy(opciones, "Verdadero, Falso");
				}
				else if (strcmp(tipo, "1") == 0)
				{
					do
					{
						cout << "Ingrese la primera opcion: ";
						cin>>opcion1;
					} while (strcmp(opcion1, "") == 0);
					do
					{
						cout << "Ingrese la segunda opcion: ";
						cin>>opcion2;
					} while (strcmp(opcion2, "") == 0);
					do
					{
						cout << "Ingrese la tercera opcion: ";
						cin>>opcion3;
					} while (strcmp(opcion3, "") == 0);

					strcpy(opciones, opcion1);
					strcat(opciones, ", ");
					strcat(opciones, opcion2);
					strcat(opciones, ", ");
					strcat(opciones, opcion3);
				}
				else
				{
					strcpy(opciones, "NULL");
				}

				send(s, opciones, strlen(opciones) + 1, 0);

				if (strcmp(tipo, "3") == 0)
				{
					cout << "Ingrese la respuesta: ";
					cin>>respuesta;
				}
				else if (strcmp(tipo, "2") == 0)
				{
					do
					{
						cout << "Ingrese la respuesta (1 para verdadero o 0 para falso): ";
						cin>>respuesta;
					} while (strcmp(respuesta, "1") != 0 && strcmp(respuesta, "0") != 0);
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
						cout << "Ingrese la respuesta (1, 2, 3): ";
						cin>>respuesta;
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

				send(s, respuesta, strlen(respuesta) + 1, 0);

				cout << "Ingrese el test para agregar la pregunta: ";
				cin>>test;
				cout << "test: " << test << endl;
				cout << "pregunta: " << pregunta << endl;
				cout << "respuesta: " << respuesta << endl;
				cout << "tipo: " << tipo << endl;
				cout << "opciones: " << opciones << endl;

				send(s, test, strlen(test) + 1, 0);
				
			}
			else
			{
				cout << "No se van a añadir preguntas." << endl;
				break;
			}

			cout << "Recepcion del mensaje..." << endl;
			recv(s, recvBuff, sizeof(recvBuff), 0);
			cout << "Datos recibidos: " << recvBuff << endl;
			break;

		case '6':
			cout << "Envio del mensaje 1..." << endl;
			strcpy(sendBuff, "resultado teses");
			send(s, sendBuff, strlen(sendBuff) + 1, 0);

			cout << "Recepcion del mensaje 1..." << endl;
			recv(s, recvBuff, sizeof(recvBuff), 0);
			cout << "Datos recibidos: " << recvBuff << endl;
			cout << endl;
			cout << "Resultados: " << endl;
			separarResultado(recvBuff);
			break;

		case '0':
			cout << "Envio del mensaje 1..." << endl;
			strcpy(sendBuff, "Fin");
			send(s, sendBuff, strlen(sendBuff) + 1, 0);

			memset(recvBuff, 0, sizeof(recvBuff));
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