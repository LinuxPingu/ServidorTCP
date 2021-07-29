#include <iostream>
#include <ws2tcpip.h>
#pragma comment (lib,"ws2_32.lib")
using namespace std;

void main() {

	// 1) Initializae Winsock *///
		
	WSADATA wsDATA;
	WORD ver = MAKEWORD(2, 2);
	int wsOK = WSAStartup(ver, &wsDATA);
	if (wsOK) {
		cerr << "Can't Initialize winsock! Quitting..." << endl;
		return;
	}

	// 2) Create a Socket *//
	SOCKET listening = socket(AF_INET,SOCK_STREAM,0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create main Socket! Quitting..." << endl;
		return;
	}

	// 3) Bind a Socket to an Ip Adress and Port *//
	 sockaddr_in hint;
	 hint.sin_family = AF_INET;
	 hint.sin_port = htons(54000);
	 hint.sin_addr.S_un.S_addr = INADDR_ANY;
	 bind(listening,(sockaddr*)&hint,sizeof(hint));

	// 4) Tell Winsock the socket is for listening *//
	
	 listen(listening,SOMAXCONN);

	// 5) Wait for Connection *//
	 sockaddr_in client;
	 int clientsize = sizeof(client);
	 SOCKET clientsocket = accept(listening, (sockaddr*)&client, &clientsize);
	 if (clientsocket == INVALID_SOCKET)
	 {
		 cerr << "Can't create client Socket! Quitting..." << endl;
		 return;
	 }

	 char host[NI_MAXHOST]; // Nombre del cliente *//
	 char service[NI_MAXHOST]; // Puerto en el que el cliente esta conectado*//

	 ZeroMemory(host,NI_MAXHOST);
	 ZeroMemory(service, NI_MAXHOST);

	 if (getnameinfo( (sockaddr*)&client,sizeof(client),host,NI_MAXHOST,service,NI_MAXSERV,0) == 0) {

		 cout << host << " conected to port " << service << endl;
	 }
	 else
	 {
		 inet_ntop(AF_INET, &client.sin_addr,host, NI_MAXHOST);
		 cout << host << " connected on port " << ntohs(client.sin_port) << endl;
	 }

	// 6) Close Listening  socket *//
	 closesocket(listening);

	// 7) While loop: accept and echo message back to client *//
	 
	 char buf[4096];

	 while (true)
	 {
		 ZeroMemory(buf,4069);

		 // Espera por un cliente a enviar informacion *//
		 int bytesRecived = recv(clientsocket, buf , 4096,0);
		 if (bytesRecived == SOCKET_ERROR)
		 {
			 cerr << "Error in recv(). Quitting..." << endl;
			 break;
		 }

		 if (bytesRecived == 0)
		 {
			 cout << " Client disconnected " << endl;
			 break;
		 }

		 // Imprime el mensaje del cliente
		 send(clientsocket, buf, bytesRecived + 1, 0);
	 }

	// 8) Close Socket *//
	 closesocket(clientsocket);
	
	// 9) Shutdown Winsock
	 WSACleanup();

}


