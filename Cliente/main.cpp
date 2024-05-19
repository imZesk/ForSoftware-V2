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

void escribirConTiempo(ofstream &archivo, const string &frase) {
    if (!archivo.is_open()) {
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
	ofstream archivo("../lib/LogCliente.txt",ios::trunc);

	if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo." << endl;
        return 1; // Salir del programa con un código de error
    }

	cout << endl
		 << "Inicializando Winsock..." << endl;
	escribirConTiempo(archivo,"[Cliente] Inicializando Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "Fallo. Codigo de error : " << WSAGetLastError() << endl;
		escribirConTiempo(archivo,"[Cliente] Fallo Inicializando Winsock.\n");
		return -1;
	}

	cout << "Inicializado." << endl;
	escribirConTiempo(archivo,"[Cliente] Winsock Inicializado.\n");

	// SOCKET creation
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		cout << "No se ha podido crear el socket :" << WSAGetLastError() << endl;
		escribirConTiempo(archivo,"[Cliente] No se ha podido crear el socket.\n");
		WSACleanup();
		return -1;
	}

	cout << "Socket creado." << endl;
	escribirConTiempo(archivo,"[Cliente] Socket creado.\n");

	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	// CONNECT to remote server
	if (connect(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		cout << "Error de conexion: " << WSAGetLastError() << endl;
		escribirConTiempo(archivo,"[Cliente] Error de conexion.\n");
		closesocket(s);
		WSACleanup();
		return -1;
	}
	cout << "Conexion establecida con: " << inet_ntoa(server.sin_addr) << " ("
		 << ntohs(server.sin_port) << ")" << endl;
	ostringstream oss;
    oss << "[Cliente] Conexion establecida con: " << inet_ntoa(server.sin_addr) << " (" << ntohs(server.sin_port) << ")";
    string texto = oss.str();
	escribirConTiempo(archivo,texto);


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
    send(s, sendBuff, strlen(sendBuff) + 1, 0);

    cout << "Recepcion del mensaje 1..." << endl;
    recv(s, recvBuff, sizeof(recvBuff), 0);
    if (strcmp(recvBuff, "Recibido.") != 0) {
        cout << "Error: No se recibió confirmación del servidor." << endl;
        break;
    }
    cout << "Datos recibidos: " << recvBuff << endl;

    char nombreEncuesta[100];
    int cantidadPreguntas;
    cout << "Introduce el nombre de la encuesta: ";
    cin.ignore();
    cin.getline(nombreEncuesta, sizeof(nombreEncuesta));
    cout << "Introduce la cantidad de preguntas: ";
    cin >> cantidadPreguntas;

    stringstream data;
    data << nombreEncuesta << "," << cantidadPreguntas;

    for (int i = 0; i < cantidadPreguntas; ++i)
    {
        int tipoPregunta;
        cout << "Elige el tipo de pregunta (1: Abierta, 2: Opción Múltiple, 3: Verdadero/Falso): ";
        cin >> tipoPregunta;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

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
            cout << "Introduce la primera opción: ";
            cin.getline(opcion1, sizeof(opcion1));
            cout << "Introduce la segunda opción: ";
            cin.getline(opcion2, sizeof(opcion2));
            cout << "Introduce la tercera opción: ";
            cin.getline(opcion3, sizeof(opcion3));
            cout << "Introduce la opción correcta: ";
            cin.getline(respuesta, sizeof(respuesta));
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
            cout << "Tipo de pregunta no válido. Intente nuevamente." << endl;
            --i;
        }
    }

    string dataStr = data.str();
    strcpy(sendBuff, dataStr.c_str());
    send(s, sendBuff, strlen(sendBuff) + 1, 0);

    // Recibir confirmación de que el test y las preguntas fueron creadas
    recv(s, recvBuff, sizeof(recvBuff), 0);
    cout << "Datos recibidos: " << recvBuff << endl;

    cout << "Encuesta '" << nombreEncuesta << "' creada con " << cantidadPreguntas << " preguntas." << endl;
    break;
}


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
					cout << "No hay mas preguntas disponibles." << endl;
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
					recv(s, recvBuff, sizeof(recvBuff)+1, 0);
					cout << "Resultado: " << recvBuff << endl;
				}
			}
			char visN[10];
			do{
				cout<<"Quieres visualizar la nota(1 si, 0 no): ";
				cin.ignore();
				cin>>visN;
			}while (strcmp(visN, "1") != 0 && strcmp(visN, "0") != 0);
			if (strcmp(visN, "1") == 0)
			{
				send(s, "Ver", strlen("Ver") + 1, 0);
				recv(s, recvBuff, sizeof(recvBuff), 0);
				cout << "Tu nota es: " << recvBuff << endl;
				break;
			}	

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
				cout << "Ingrese el tipo de pregunta\n1: pregunta con opcion multiple\n2: Verdadero o Falso\n3: Pregunta abierta\n0: Si no quieres añadir ninguna pregunta: ";
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
			escribirConTiempo(archivo,"[Cliente] Fin del programa.\n");
			break;

		default:
			cout << "ERROR, introduce de nuevo" << endl;
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	} while (opcion != '0');

	// CLOSING the socket and cleaning Winsock...
	closesocket(s);
	WSACleanup();
	archivo.close();

	return 0;
}