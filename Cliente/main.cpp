#include <iostream>
#include <winsock2.h>
#include <cstring>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

using namespace std;

int main(int argc, char *argv[])
{
    WSADATA wsaData;
    SOCKET s;
    struct sockaddr_in server;
    char sendBuff[512], recvBuff[512];

    cout << "\nInitialising Winsock...\n";
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cout << "Failed. Error Code : " << WSAGetLastError() << endl;
        return -1;
    }

    cout << "Initialised."<<endl;

    //SOCKET creation
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		cout << "Could not create socket :"<<WSAGetLastError()<<endl;
		WSACleanup();
		return -1;
	}

	cout<<"Socket created."<<endl;

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

    //CONNECT to remote server
	if (connect(s, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
		cout<<"Connection error: "<< WSAGetLastError()<<endl;
		closesocket(s);
		WSACleanup();
		return -1;
	}
    cout<<"Connection stablished with: "<<inet_ntoa(server.sin_addr)<<" ("
    <<ntohs(server.sin_port)<<")"<<endl;

    // SEND and RECEIVE data
	cout<<"Sending message 1..."<<endl;
    strcpy(sendBuff, "Hello, server.");
	send(s, sendBuff, sizeof(sendBuff), 0);

	cout<<"Receiving message 1..."<<endl;
	recv(s, recvBuff, sizeof(recvBuff), 0);
	cout<<"Data received: "<< recvBuff<<endl;

	cout<<"Sending message 2..."<<endl;
	strcpy(sendBuff, "Hello again.");
	send(s, sendBuff, sizeof(sendBuff), 0);
	cout<<"Data sent: "<<sendBuff<<endl;

	cout<<"Receiving message 2..."<<endl;
	recv(s, recvBuff, sizeof(recvBuff), 0);
	cout<<"Data received: "<<recvBuff<<endl;

	cout<<"Sending last message..."<<endl;
	strcpy(sendBuff, "Bye");
	send(s, sendBuff, sizeof(sendBuff), 0);
	cout<<"Data sent: "<<sendBuff<<endl;

    // CLOSING the socket and cleaning Winsock...
    closesocket(s);
    WSACleanup();

    return 0;
}