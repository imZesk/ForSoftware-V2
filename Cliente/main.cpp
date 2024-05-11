#include <iostream>
#include <winsock2.h>
#include <cstring>
#include "menus.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

using namespace std;

int main(int argc, char *argv[])
{
    WSADATA wsaData;
    SOCKET s;
    struct sockaddr_in server;
    char sendBuff[512], recvBuff[512];

    cout << endl<<"Inicializando Winsock..."<<endl;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cout << "Fallo. Codigo de error : " << WSAGetLastError() << endl;
        return -1;
    }

    cout << "Inicializado."<<endl;

    //SOCKET creation
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		cout << "No se ha podido crear el socket :"<<WSAGetLastError()<<endl;
		WSACleanup();
		return -1;
	}

	cout<<"Socket creado."<<endl;

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

    //CONNECT to remote server
	if (connect(s, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
		cout<<"Error de conexion: "<< WSAGetLastError()<<endl;
		closesocket(s);
		WSACleanup();
		return -1;
	}
    cout<<"Conexion establecida con: "<<inet_ntoa(server.sin_addr)<<" ("
    <<ntohs(server.sin_port)<<")"<<endl;

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
			cout<<"Envio del mensaje 1..."<<endl;
    		strcpy(sendBuff, "Crear test.");
			send(s, sendBuff, sizeof(sendBuff), 0);

			cout<<"Recepcion del mensaje 1..."<<endl;
			recv(s, recvBuff, sizeof(recvBuff), 0);
			cout<<"Datos recibidos: "<< recvBuff<<endl;
            break;

        case '2':
			cout<<"Envio del mensaje 1..."<<endl;
    		strcpy(sendBuff, "Realizar test.");
			send(s, sendBuff, sizeof(sendBuff), 0);

			cout<<"Recepcion del mensaje 1..."<<endl;
			recv(s, recvBuff, sizeof(recvBuff), 0);
			cout<<"Datos recibidos: "<< recvBuff<<endl;
            break;

        case '3':
			cout<<"Envio del mensaje 1..."<<endl;
    		strcpy(sendBuff, "Visualizar test.");
			send(s, sendBuff, sizeof(sendBuff), 0);

			cout<<"Recepcion del mensaje 1..."<<endl;
			recv(s, recvBuff, sizeof(recvBuff), 0);
			cout<<"Datos recibidos: "<< recvBuff<<endl;
            break;

        case '4':
			cout<<"Envio del mensaje 1..."<<endl;
    		strcpy(sendBuff, "Eliminar test.");
			send(s, sendBuff, sizeof(sendBuff), 0);

			cout<<"Recepcion del mensaje 1..."<<endl;
			recv(s, recvBuff, sizeof(recvBuff), 0);
			cout<<"Datos recibidos: "<< recvBuff<<endl;
            break;

        case '0':
			cout<<"Envio del mensaje 1..."<<endl;
    		strcpy(sendBuff, "Fin");
			send(s, sendBuff, sizeof(sendBuff), 0);

			cout<<"Recepcion del mensaje 1..."<<endl;
			recv(s, recvBuff, sizeof(recvBuff), 0);
			cout<<"Datos recibidos: "<< recvBuff<<endl;

            cout<<"Fin del programa"<<endl;
            break;

        default:
            cout<<"ERROR, introduce de nuevo"<<endl;
        }
    } while (opcion != '0');

    // CLOSING the socket and cleaning Winsock...
    closesocket(s);
    WSACleanup();

    return 0;
}