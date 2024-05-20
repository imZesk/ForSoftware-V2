#include <iostream>
#include <winsock2.h>
#include <cstring>
#include "menus.h"
#include "Pregunta/encuesta.h"
#include <limits>
#include <sstream>
#include <string>
#include <list>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include "Pregunta/PreguntaOpcionMultiple.h"
#include "Pregunta/PreguntaAbierta.h"
#include "Pregunta/PreguntaVerdaderoFalso.h"
#include "Pregunta/Pregunta.h"

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
		int id_t=0;
		const char *nombre;
		while (getline(grupo_ss, palabra, ','))
		{
			if (contador == 0)
			{
				nombre_encuesta = palabra;
			}
			else if (contador == 1)
			{
				id_t = stoi(palabra);
			}
			else if (contador == 2)
			{
				preg_realiza = stoi(palabra);
			}
			else if(contador==3)
			{
				respuestas_c = stoi(palabra);
			}
			contador++;
		}

		nombre = nombre_encuesta.c_str();
		// Crear una nueva encuesta con los valores obtenidos
		if(respuestas_c!=0){
			Encuesta nueva_encuesta(preg_realiza);
			nueva_encuesta.setNombre(nombre);
			nueva_encuesta.setNota(respuestas_c);
			nueva_encuesta.setId(id_t);
			cout << "Nombre: " << nueva_encuesta.getNombre()<<" " << nueva_encuesta.getNota() << "/" << nueva_encuesta.getCantidadPreguntas() << endl;
			cout << endl;
		}
	}
}

void escribirConTiempo(ofstream &archivo, const string &frase)
{
	if (!archivo.is_open())
	{
		cerr << "Error: el archivo no está abierto." << endl;
		return;
	}

	// Obtener la hora actual
	time_t t = time(nullptr);
	tm tm = *localtime(&t);

	// Escribir la hora y la frase en el archivo
	archivo << "[" << put_time(&tm, "%H:%M:%S") << "] " << frase << endl;
}

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	SOCKET s;
	struct sockaddr_in server;
	char sendBuff[512], recvBuff[512];
	ofstream archivo("../lib/LogCliente.txt", ios::trunc);

	if (!archivo.is_open())
	{
		cerr << "Error al abrir el archivo." << endl;
		return 1; // Salir del programa con un código de error
	}

	cout << endl
		 << "Inicializando Winsock..." << endl;
	escribirConTiempo(archivo, "[Cliente] Inicializando Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "Fallo. Codigo de error : " << WSAGetLastError() << endl;
		escribirConTiempo(archivo, "[Cliente] Fallo Inicializando Winsock.\n");
		return -1;
	}

	cout << "Inicializado." << endl;
	escribirConTiempo(archivo, "[Cliente] Winsock Inicializado.\n");

	// SOCKET creation
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		cout << "No se ha podido crear el socket :" << WSAGetLastError() << endl;
		escribirConTiempo(archivo, "[Cliente] No se ha podido crear el socket.\n");
		WSACleanup();
		return -1;
	}

	cout << "Socket creado." << endl;
	escribirConTiempo(archivo, "[Cliente] Socket creado.\n");

	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	// CONNECT to remote server
	if (connect(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		cout << "Error de conexion: " << WSAGetLastError() << endl;
		escribirConTiempo(archivo, "[Cliente] Error de conexion.\n");
		closesocket(s);
		WSACleanup();
		return -1;
	}
	cout << "Conexion establecida con: " << inet_ntoa(server.sin_addr) << " ("
		 << ntohs(server.sin_port) << ")" << endl;
	ostringstream oss;
	oss << "[Cliente] Conexion establecida con: " << inet_ntoa(server.sin_addr) << " (" << ntohs(server.sin_port) << ")";
	string texto = oss.str();
	escribirConTiempo(archivo, texto);

	escribirConTiempo(archivo, "[Cliente] Llegue\n");

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
	string texto8;
	string texto1;
	string texto2;
	string texto3;
	string texto4;
	string texto5;
	string texto6;
	string texto7;
	ostringstream oss8;
	ostringstream oss1;
	ostringstream oss2;
	ostringstream oss3;
	ostringstream oss4;
	ostringstream oss5;
	ostringstream oss6;
	ostringstream oss7;
	list<string> nombresEncuestas;
	bool encontrado = false;

	Encuesta nuevaEncuesta;
	do
	{
		opcion = menuPrincipal();

		switch (opcion)
		{
		case '1':
		{
			cout << "Envio del mensaje 1..." << endl;
			strcpy(sendBuff, "Crear test.");
			oss8 << "[Cliente] Opcion elegida: " << sendBuff << endl;
			texto8 = oss8.str();
			escribirConTiempo(archivo, texto8);
			send(s, sendBuff, strlen(sendBuff) + 1, 0);

			cout << "Recepcion del mensaje 1..." << endl;
			recv(s, recvBuff, sizeof(recvBuff), 0);
			if (strcmp(recvBuff, "Recibido.") != 0)
			{
				cout << "Error: No se recibio confirmacion del servidor." << endl;
				escribirConTiempo(archivo, "[Cliente] Error: No se recibio confirmacion del servidor.\n");
				break;
			}
			cout << "Datos recibidos: " << recvBuff << endl;
			oss2 << "[Cliente] Datos recibidos: " << recvBuff << endl;
			texto2 = oss2.str();
			escribirConTiempo(archivo, texto2);

			char nombreEncuesta[100];
			int cantidadPreguntas;
			cout << "Introduce el nombre del test: ";
			cin.ignore();
			cin.getline(nombreEncuesta, sizeof(nombreEncuesta));

			while (true)
			{
				cout << "Introduce la cantidad de preguntas: ";
				cin >> cantidadPreguntas;
				if (cin.fail() || cantidadPreguntas < 1 || cantidadPreguntas > 30)
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Error: Introduce un numero entre 1 y 30." << endl;
				}
				else
				{
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					break;
				}
			}

			stringstream data;
			data << nombreEncuesta << "," << cantidadPreguntas;

			for (int i = 0; i < cantidadPreguntas; ++i)
			{
				int tipoPregunta;

				while (true)
				{
					cout << "Elige el tipo de pregunta (1: Abierta, 2: Opcion Multiple, 3: Verdadero/Falso): ";
					cin >> tipoPregunta;
					if (cin.fail() || tipoPregunta < 1 || tipoPregunta > 3)
					{
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "Error: Introduce un numero entre 1 y 3." << endl;
					}
					else
					{
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						break;
					}
				}

				char textoPregunta[512];
				cout << "Introduce la pregunta: ";
				cin.getline(textoPregunta, sizeof(textoPregunta));

				if (tipoPregunta == 1)
				{
					char respuesta[512];
					cout << "Introduce la respuesta: ";
					cin.getline(respuesta, sizeof(respuesta));
					data << ";" << tipoPregunta << "," << textoPregunta << ",NULL," << respuesta;
				}
				else if (tipoPregunta == 2)
				{
					char opcion1[512], opcion2[512], opcion3[512];
					char respuesta[512];
					cout << "Introduce la primera opcion: ";
					cin.getline(opcion1, sizeof(opcion1));
					cout << "Introduce la segunda opcion: ";
					cin.getline(opcion2, sizeof(opcion2));
					cout << "Introduce la tercera opcion: ";
					cin.getline(opcion3, sizeof(opcion3));

					while (true)
					{
						cout << "Introduce la opcion correcta (1, 2 o 3): ";
						cin.getline(respuesta, sizeof(respuesta));
						if (strlen(respuesta) != 1 || respuesta[0] < '1' || respuesta[0] > '3')
						{
							cout << "Error: Introduce un numero entre 1 y 3." << endl;
						}
						else
						{
							break;
						}
					}
					data << ";" << tipoPregunta << "," << textoPregunta << "," << opcion1 << "|" << opcion2 << "|" << opcion3 << "," << respuesta;
				}
				else if (tipoPregunta == 3)
				{
					char respuesta;
					do
					{
						cout << "Introduce la respuesta (V/F): ";
						cin >> respuesta;
						respuesta = toupper(respuesta);
					} while (respuesta != 'V' && respuesta != 'F');
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					data << ";" << tipoPregunta << "," << textoPregunta << ",NULL," << respuesta;
				}
				else
				{
					cout << "Tipo de pregunta no valido. Intente nuevamente." << endl;
					--i;
				}
			}

			string dataStr = data.str();
			strcpy(sendBuff, dataStr.c_str());
			oss3 << "[Cliente] Datos enviados: " << sendBuff << endl;
			texto3 = oss3.str();
			escribirConTiempo(archivo, texto3);
			send(s, sendBuff, strlen(sendBuff) + 1, 0);

			// Recibir confirmación de que el test y las preguntas fueron creadas
			recv(s, recvBuff, sizeof(recvBuff), 0);
			cout << "Datos recibidos: " << recvBuff << endl;
			oss4 << "[Cliente] Datos recibidos: " << recvBuff << endl;
			texto4 = oss4.str();
			escribirConTiempo(archivo, texto4);

			cout << "Test '" << nombreEncuesta << "' creada con " << cantidadPreguntas << " preguntas." << endl;
			break;
		}

		case '2':
			cout << "Realizando test..." << endl;
			strcpy(sendBuff, "Realizar test.");
			oss8 << "[Cliente] Opcion elegida: " << sendBuff <<endl;
    		texto8 = oss8.str();
			escribirConTiempo(archivo,texto8);
			send(s, sendBuff, strlen(sendBuff) + 1, 0);

			cout << "Que test desea realizar: ";
			cin >> sendBuff;
			oss2 << "[Cliente] Datos enviados: " << sendBuff <<endl;
    		texto2 = oss2.str();
			escribirConTiempo(archivo,texto2);
			send(s, sendBuff, strlen(sendBuff) + 1, 0);

			while (true)
			{
				// Recibir pregunta y opciones del servidor
				recv(s, recvBuff, sizeof(recvBuff) + 1, 0);

				if (strcmp(recvBuff, "No hay mas preguntas disponibles.") == 0)
				{
					cout << "No hay mas preguntas disponibles." << endl;
					escribirConTiempo(archivo,"[Cliente] No hay mas preguntas disponibles.\n");
					break;
				}
				else if (strcmp(recvBuff, "El test no existe") == 0)
				{
					cout << "El test no existe" << endl;
					escribirConTiempo(archivo,"[Cliente] El test no existe.\n");
					break;
				}
				else
				{
					oss3 << "[Cliente] Datos recibidos: " << recvBuff <<endl;
    				texto3 = oss3.str();
					escribirConTiempo(archivo,texto3);
					cout << recvBuff << endl; // Mostrar la pregunta y las opciones

					cout << "Ingrese su respuesta: ";
					cin >> respuesta;
					strcpy(sendBuff, respuesta);
					oss4 << "[Cliente] Datos enviados: " << sendBuff <<endl;
    				texto4 = oss4.str();
					escribirConTiempo(archivo,texto4);
					send(s, sendBuff, strlen(sendBuff) + 1, 0);

					// Recibir resultado del servidor
					recv(s, recvBuff, sizeof(recvBuff) + 1, 0);
					cout << "Resultado: " << recvBuff << endl;
					oss5 << "[Cliente] Datos recibidos: " << recvBuff <<endl;
    				texto5 = oss5.str();
					escribirConTiempo(archivo,texto5);
				}
			}
			char visN[10];
			do
			{
				cout << "Quieres visualizar la nota(1 si, 0 no): ";
				cin.ignore();
				cin >> visN;
			} while (strcmp(visN, "1") != 0 && strcmp(visN, "0") != 0);
			if (strcmp(visN, "1") == 0)
			{
				escribirConTiempo(archivo,"[Cliente] Datos enviados: Ver\n");
				send(s, "Ver", strlen("Ver") + 1, 0);
				recv(s, recvBuff, sizeof(recvBuff), 0);
				cout << "Tu nota es: " << recvBuff << endl;
				oss6 << "[Cliente] Datos recibidos: " << recvBuff <<endl;
    			texto6 = oss6.str();
				escribirConTiempo(archivo,texto6);
				break;
			}

		case '3':
			cout << "Envio del mensaje 1..." << endl;
			strcpy(sendBuff, "Visualizar test.");
			oss8 << "[Cliente] Datos enviados: " << sendBuff <<endl;
    		texto8 = oss8.str();
			escribirConTiempo(archivo,texto8);
			send(s, sendBuff, strlen(sendBuff) + 1, 0);

			cout << "Recepcion del mensaje 1..." << endl;
			recv(s, recvBuff, sizeof(recvBuff), 0);
			cout << "Datos recibidos: " << recvBuff << endl;
			oss2 << "[Cliente] Datos recibidos: " << recvBuff <<endl;
    		texto2 = oss2.str();
			escribirConTiempo(archivo,texto2);
			cout << endl;
			cout << "Teses: " << endl;
			separarPalabras(recvBuff);
			break;

		case '4':
			cout << "Envio del mensaje 1..." << endl;
			strcpy(sendBuff, "Eliminar test.");
			oss8 << "[Cliente] Datos enviados: " << sendBuff <<endl;
    		texto8 = oss8.str();
			escribirConTiempo(archivo,texto8);
			send(s, sendBuff, strlen(sendBuff) + 1, 0);

			cout << "Recepcion del mensaje 1..." << endl;
			recv(s, recvBuff, sizeof(recvBuff), 0);
			oss2 << "[Cliente] Datos recibidos: " << recvBuff <<endl;
    		texto2 = oss2.str();
			escribirConTiempo(archivo,texto2);
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
			oss3 << "[Cliente] Datos enviados: " << sendBuff <<endl;
    		texto3 = oss3.str();
			escribirConTiempo(archivo,texto3);
			send(s, sendBuff, strlen(sendBuff) + 1, 0);
			cout << "Recepcion del mensaje 2..." << endl;
			recv(s, recvBuff, sizeof(recvBuff), 0);
			cout << "Datos recibidos: " << recvBuff << endl;
			oss4 << "[Cliente] Datos recibidos: " << recvBuff <<endl;
    		texto4 = oss4.str();
			escribirConTiempo(archivo,texto4);
			break;

		case '5':
			cout << "Envio del mensaje 1..." << endl;
			strcpy(sendBuff, "Agregar Pregunta.");
			oss8 << "[Cliente] Datos enviados: " << sendBuff <<endl;
    		texto8 = oss8.str();
			escribirConTiempo(archivo,texto8);
			send(s, sendBuff, strlen(sendBuff) + 1, 0);

			do
			{
				cout << "Ingrese el tipo de pregunta\n1: pregunta con opcion multiple\n2: Verdadero o Falso\n3: Pregunta abierta\n0: Si no quieres añadir ninguna pregunta: ";
				cin >> tipo;
			} while (strcmp(tipo, "1") != 0 && strcmp(tipo, "2") != 0 && strcmp(tipo, "3") != 0 && strcmp(tipo, "0") != 0);

			oss2 << "[Cliente] Datos enviados: " << tipo <<endl;
    		texto2 = oss2.str();
			escribirConTiempo(archivo,texto2);
			send(s, tipo, strlen(tipo) + 1, 0);

			if (strcmp(tipo, "0") != 0)
			{
				cout << "Ingrese la pregunta: ";
				cin.ignore();
				string pregunta;
				getline(cin, pregunta);

				char preg[512];
				strcpy(preg, pregunta.c_str());

				cout << preg << endl;

				oss3 << "[Cliente] Datos enviados: " << preg <<endl;
    			texto3 = oss3.str();
				escribirConTiempo(archivo,texto3);
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
						cin >> opcion1;
					} while (strcmp(opcion1, "") == 0);
					do
					{
						cout << "Ingrese la segunda opcion: ";
						cin >> opcion2;
					} while (strcmp(opcion2, "") == 0);
					do
					{
						cout << "Ingrese la tercera opcion: ";
						cin >> opcion3;
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

				oss4 << "[Cliente] Datos enviados: " << opciones <<endl;
    			texto4 = oss4.str();
				escribirConTiempo(archivo,texto4);
				send(s, opciones, strlen(opciones) + 1, 0);

				if (strcmp(tipo, "3") == 0)
				{
					cout << "Ingrese la respuesta: ";
					cin >> respuesta;
				}
				else if (strcmp(tipo, "2") == 0)
				{
					do
					{
						cout << "Ingrese la respuesta (1 para verdadero o 0 para falso): ";
						cin >> respuesta;
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
						cin >> respuesta;
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

				oss5 << "[Cliente] Datos enviados: " << respuesta <<endl;
    			texto5 = oss5.str();
				escribirConTiempo(archivo,texto5);
				send(s, respuesta, strlen(respuesta) + 1, 0);

				cout << "Ingrese el test para agregar la pregunta: ";
				cin >> test;
				
				oss6 << "[Cliente] Datos enviados: " << test <<endl;
    			texto6 = oss6.str();
				escribirConTiempo(archivo,texto6);
				send(s, test, strlen(test) + 1, 0);
			}
			else
			{
				cout << "No se van a agregar preguntas." << endl;
				break;
			}

			cout << "Recepcion del mensaje..." << endl;
			recv(s, recvBuff, sizeof(recvBuff), 0);
			cout << "Datos recibidos: " << recvBuff << endl;
			oss7 << "[Cliente] Datos recibidos: " << recvBuff <<endl;
    		texto7 = oss7.str();
			escribirConTiempo(archivo,texto7);
			break;

		case '6':
			cout << "Envio del mensaje 1..." << endl;
			strcpy(sendBuff, "resultado teses");
			oss8 << "[Cliente] Datos enviados: " << sendBuff <<endl;
    		texto8 = oss8.str();
			escribirConTiempo(archivo,texto8);
			send(s, sendBuff, strlen(sendBuff) + 1, 0);

			cout << "Recepcion del mensaje 1..." << endl;
			recv(s, recvBuff, sizeof(recvBuff), 0);
			oss2 << "[Cliente] Datos recibidos: " << recvBuff <<endl;
    		texto2 = oss2.str();
			escribirConTiempo(archivo,texto2);
			cout << "Datos recibidos: " << recvBuff << endl;
			cout << endl;
			cout << "Resultados: " << endl;
			separarResultado(recvBuff);
			break;

		case '0':
			cout << "Envio del mensaje 1..." << endl;
			strcpy(sendBuff, "Fin");
			
			oss8 << "[Cliente] Datos enviados: " << sendBuff <<endl;
    		texto8 = oss8.str();
			escribirConTiempo(archivo,texto8);
			send(s, sendBuff, strlen(sendBuff) + 1, 0);

			memset(recvBuff, 0, sizeof(recvBuff));
			cout << "Recepcion del mensaje 1..." << endl;
			recv(s, recvBuff, sizeof(recvBuff), 0);
			cout << "Datos recibidos: " << recvBuff << endl;
			
			oss2 << "[Cliente] Datos recibidos: " << recvBuff <<endl;
    		texto2 = oss2.str();
			escribirConTiempo(archivo,texto2);

			cout << "Fin del programa" << endl;
			escribirConTiempo(archivo, "[Cliente] Fin del programa.\n");
			break;

		default:
			cout << "ERROR, introduce de nuevo" << endl;
			escribirConTiempo(archivo,"[Cliente] ERROR, introduce de nuevo.\n");
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	} while (opcion != '0');

	// CLOSING the socket and cleaning Winsock...
	closesocket(s);
	escribirConTiempo(archivo,"[Cliente] Socket cerrado.\n");
	WSACleanup();
	escribirConTiempo(archivo,"[Cliente] Winsock limpiado.\n");
	archivo.close();

	return 0;
}